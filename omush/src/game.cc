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
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/action/action.h"
#include "omush/notifier.h"
#include "omush/database/targetmatcher.h"

namespace omush {

  class WelcomeScreenCommandParser : public CommandParser {
   public:
    WelcomeScreenCommandParser() {
    nodes_ = new CommandNode();
      registerCommand(new CommandQuit());
      registerCommand(new CommandConnect());
    }
  };

  class DescriptorCommandParser : public CommandParser {
  public:
    DescriptorCommandParser() {
    nodes_ = new CommandNode();
      registerCommand(new CommandQuit());
    }
  };

  class HCCommandParser : public CommandParser {
  public:
    HCCommandParser() {
    nodes_ = new CommandNode();
      registerCommand(new CommandHuh());
      registerCommand(new CommandLook());
      registerCommand(new CommandSay());
      registerCommand(new CommandPose());
      registerCommand(new CommandGo());
      registerCommand(new CommandThink());
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
      return "x1b[color:red]Commands:x1b[end]\n  connect <name> <password>\n  create <name> <password>\n  HELP\n  QUIT";
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

    return false;
  }

  bool Game::processCommands() {
    if (commandList_.empty())
      return false;

    InternalCommand command = commandList_.front();
    commandList_.pop();

    CommandContext context;
    context.game = this;
    context.db = db_;
    context.dbref = command.dbref;

    HCCommandParser cmds = HCCommandParser();
    if (!cmds.run(command.cmd, context)) {
      !cmds.run("HUH", context);
    }

    return true;
  }

  bool Game::handleIncommingMessages() {
    unsigned long numberOfMessages = server_->getIncommingMessageCount();
    if (numberOfMessages > 0) {
      network::IncommingMessage msg = server_->popIncommingMessage();

      if (clientList_.find(msg.id) == clientList_.end())
        return false;

      Client *c = &(clientList_[msg.id]);

      CommandContext context;
      context.game = this;
      context.descriptor = msg.id;
      context.client = c;
      context.db = db_;

      if (c->isConnected == true) {
        DescriptorCommandParser descCmds = DescriptorCommandParser();
        if (!descCmds.run(msg.rawString, context)) {
          commandList_.push(InternalCommand(c->dbref,msg.rawString));
        }
      } else {
        WelcomeScreenCommandParser cmds = WelcomeScreenCommandParser();
        if (!cmds.run(msg.rawString, context)) {
          // Multiple cases.
          sendNetworkMessage(msg.id, "I don't recognize that command.");
          sendNetworkMessage(msg.id, WelcomeScreen::getRandomText());
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
    sendNetworkMessage(id, "Going Down");
    server_->closeNetworkConnection(id);
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
    db_ = new database::Database();

    database::DatabaseObject *r1=database::DatabaseObjectFactory::createRoom(db_);
    database::DatabaseObject *p1=database::DatabaseObjectFactory::createPlayer(db_);
    database::DatabaseObject *r2=database::DatabaseObjectFactory::createRoom(db_);
    database::DatabaseObject *p2=database::DatabaseObjectFactory::createPlayer(db_);
    database::DatabaseObject *e1=database::DatabaseObjectFactory::createExit(db_);
    database::DatabaseObject *e2=database::DatabaseObjectFactory::createExit(db_);
    r1->setProperty("name", "Room Zero");
    p1->setProperty("name", "One");
    r2->setProperty("name", "Room Two");
    p2->setProperty("name", "Michael");
    e1->setProperty("name", "Out");
    e2->setProperty("name", "Out");
    db_->moveObject(p1, r1);
    db_->moveObject(p2, r1);
    e1->home(r2->dbref());
    e2->home(r1->dbref());
    db_->moveObject(e1, r1);
    db_->moveObject(e2, r2);


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
    shutdown();

    delete db_;
    delete server_;
  }

  void Game::shutdown() {
    std::map<network::ConnectionId, Client>::iterator iter;
    for (iter = clientList_.begin();
         iter != clientList_.end();
         ++iter) {
      closeNetworkConnection((iter->first));
    }
    server_->poll();

    shutdown_ = true;
  }

  void Game::handleSignal(int signum) {
    shutdown();
  }

}  // namespace omush
