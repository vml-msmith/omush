/**
 * \file game.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/game.h"


#import "signal.h"
#import "time.h"

#import <stdio.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <queue>
#include <vector>

#include "omush/gametimer.h"
#include "omush/network/networkservice.h"
#include "omush/command.h"
#include "omush/utility.h"
#include "omush/colorstring.h"
#include "omush/database/database.h"
#include "omush/database/helpers.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/action/action.h"
#include "omush/notifier.h"
#include "omush/database/targetmatcher.h"
#include "omush/function/function.h"
#include "omush/database/databasefactory.h"
#include "omush/database/storage.h"

namespace omush {

  class WelcomeScreenCommandParser : public CommandParser {
  public:
    WelcomeScreenCommandParser() {
      registerMatcher(new CommandMatcherAbsolute());
      registerMatcher(new CommandMatcherPattern());
      registerCommand(new CommandQuit());
      registerCommand(new CommandConnect());
      registerCommand(new CommandCreatePlayer());
      registerCommand(new CommandWho());
    }
  };


  class DescriptorCommandParser : public CommandParser {
  public:
    DescriptorCommandParser() {
      registerMatcher(new CommandMatcherAbsolute());
      registerCommand(new CommandQuit());
      registerCommand(new CommandWho());
    }
  };

 class HCCommandParser : public CommandParser {
  public:
   HCCommandParser() {
     std::map<std::string,std::string> attributes;
     attributes.insert(std::make_pair<std::string,std::string>("DESCRIPTION", "DESCRIPTION"));
     attributes.insert(std::make_pair<std::string,std::string>("DESC", "DESCRIPTION"));
     attributes.insert(std::make_pair<std::string,std::string>("DESCRIBE", "DESCRIPTION"));
     attributes.insert(std::make_pair<std::string,std::string>("ALIAS", "ALIAS"));

     registerMatcher(new CommandMatcherAbsolute());
     registerMatcher(new CommandMatcherPattern());
     registerMatcher(new CommandMatcherExit());
     registerMatcher(new CommandMatcherAttributeSetter(attributes));
     registerMatcher(new CommandMatcherPartial());
     registerMatcher(new CommandMatcherUserDefined());

     registerCommand(new CommandHuh());
     registerCommand(new CommandLook());
     registerCommand(new CommandExamine());
     registerCommand(new CommandSay());
     registerCommand(new CommandPose());
     registerCommand(new CommandGo());
     registerCommand(new CommandThink());
     registerCommand(new CommandSet());
     registerCommand(new CommandDig());
     registerCommand(new CommandCreate());
     registerCommand(new CommandInventory());
     registerCommand(new CommandPower());
     registerCommand(new CommandDivision());
   }
  };


  class WelcomeScreen {
   public:
    static std::string getRandomText() {
      return getRandomHeader() + std::string("\n") + getInstructions();
    }

    static std::string getRandomHeader() {
      return "---\n\n---";
    }

    static std::string getInstructions() {
      return "Commands:\n  connect <name> <password>\n  create <name> <password>\n  HELP\n  QUIT";
    }

  };


  Game::Game() : shutdown_(false) {
  }

  Game::~Game() {
  }

  bool Game::handleNewConnections() {
    server_->poll();
    std::vector<network::ConnectionId> connections = server_->connections();
    std::vector<network::ConnectionId> newConnections;
    std::vector<network::ConnectionId> closedConnections;

    for (std::vector<network::ConnectionId>::iterator it = connections.begin();
         it != connections.end();
         ++it) {
      if (clientList_.find(*it) == clientList_.end()) {
        newConnections.push_back(*it);
        Client newClient;
        clientList_.insert(std::make_pair(*it, newClient));
        sendNetworkMessage(*it, WelcomeScreen::getRandomText());
      }
    }

    if (newConnections.size() > 0) {
      std::cout << "New Connections: " << newConnections.size() << std::endl;
    }

    while (!quitList_.empty()) {
      server_->closeNetworkConnection(quitList_.front());
      quitList_.pop();
    }

    return false;
  }

  bool Game::processCommands() {
    if (commandList_.empty())
      return false;

    InternalCommand command = commandList_.front();
    commandList_.pop();


    HCCommandParser cmds = HCCommandParser();

    FunctionScope* s = new FunctionScope();
    s->enactor = command.context.cmdScope.enactor;
    s->caller = command.context.cmdScope.caller;
    s->executor = command.context.cmdScope.executor;
    command.context.funcScope = s;

    ColorString colorStringCmd = processExpression(ColorString(command.cmd), s, 1);
    command.cmd = colorStringCmd.basicString();

    ICommand* cmd = cmds.findCommand(command.cmd, command.context);

    if (cmd == NULL) {
      command.cmd = "HUH";
      commandList_.push(command);
    }
    else {
      cmd->run(command.context);
    }

    delete s;

    return true;
  }

  bool Game::handleIncommingMessages() {
    unsigned long numberOfMessages = server_->getIncommingMessageCount();
    if (numberOfMessages > 0) {
      network::IncommingMessage msg = server_->popIncommingMessage();

      if (clientList_.find(msg.id) == clientList_.end())
        return false;

      Client *c = &(clientList_[msg.id]);
      c->lastCommandTime = boost::posix_time::second_clock::universal_time();

      CommandContext context;
      context.game = this;
      context.descriptor = msg.id;
      context.client = c;
      context.db = db_;
      context.cmdScope.originalString = msg.rawString;
      context.cmdScope.currentString = msg.rawString;

      if (c->isConnected == true) {
        context.cmdScope.caller = dbrefToObject(*db_, c->dbref);
        context.cmdScope.executor = dbrefToObject(*db_, c->dbref);
        context.cmdScope.enactor = dbrefToObject(*db_, c->dbref);

        context.caller = c->dbref;
        context.executor = c->dbref;
        context.enactor = c->dbref;

        DescriptorCommandParser cmds = DescriptorCommandParser();
        ICommand* cmd = cmds.findCommand(msg.rawString, context);

        if (cmd == NULL) {
          commandList_.push(InternalCommand(context, msg.rawString));
        }
        else {
          cmd->run(context);
          //          cmd->run(msg.rawString, msg.rawString, context);
        }
      } else {
        WelcomeScreenCommandParser cmds = WelcomeScreenCommandParser();
        ICommand* cmd = cmds.findCommand(msg.rawString, context);

        if (cmd == NULL) {
          sendNetworkMessage(msg.id, "I don't recognize that command.");
          sendNetworkMessage(msg.id, WelcomeScreen::getRandomText());
        }
        else {
          cmd->run(context);
          //          cmd->run(msg.rawString, msg.rawString, context);
        }

      }

      return true;
    }

    return false;
  }

  void Game::sendNetworkMessage(network::ConnectionId id,
                                std::string message) {
    std::queue<std::string> msgQueue = encodeString(message);

    while (!msgQueue.empty()) {
      ColorString msgString = ColorString(msgQueue.front());
      msgQueue.pop();
      network::NetworkMessage msg;
      msg.id = id;
      msg.rawString = msgString.outString();
      server_->pushMessage(msg);
    }
  }

  void Game::closeNetworkConnection(network::ConnectionId id) {
    quitList_.push(id);
    if (clientList_.find(id) != clientList_.end()) {
      clientList_.erase(id);
    }
    //    sendNetworkMessage(id, "Going Down");
    //    server_->closeNetworkConnection(id);
  }

  bool Game::inShutdown() {
    return shutdown_;
  }

  void Game::run() {
    // Setup signals.
    SignalHandler::setupSignalHandling();
    SignalHandler::registerDelegate(this, SIGINT);

    network::NetworkServiceConfig options;
    options.port = 1701;

    server_ = new network::NetworkService(options);




    db_ = database::DatabaseFactory::createDatabase();

    GameTimer timer(.05f, 0);
    timer.registerInterupt(boost::bind(&Game::inShutdown, this));
    timer.registerCallback(0.001f,
                           boost::bind(&omush::Game::handleNewConnections,
                                       this));

    timer.registerCallback(0.001f,
                           boost::bind(&omush::Game::handleIncommingMessages,
                                       this));

    timer.registerCallback(0.001f,
                           boost::bind(&omush::Game::processCommands,
                                       this));

    server_->start();
    timer.run();
    /*
    database::Storage storage;
    if (!storage.openFile("objects.db"))
      return;

    storage.createTables();

    for (int i = 0; i < db_->getNextDbref(); ++i) {
      database::DatabaseObject* object = db_->findObjectByDbref(i);
      if (object == NULL)
        continue;

      //database::ObjectPersister p(db_, object);
      //      p.write(storage);
    }
    storage.closeFile();
    */

    delete db_;
    delete server_;
  }

  void Game::shutdown() {
    shutdown_ = true;
  }

  void Game::handleSignal(int signum) {
    shutdown();
  }

}  // namespace omush
