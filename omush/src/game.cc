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

namespace omush {

  class WelcomeScreenCommandParser : public CommandParser {
   public:
    WelcomeScreenCommandParser() {
      registerCommand(new CommandQuit());
      registerCommand(new CommandConnect());
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

      if (c->isConnected == true) {
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

  void Game::sendNetworkMessage(network::ConnectionId id, std::string message) {


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

  std::queue<std::string> Game::encodeString(std::string message) {
    std::queue<std::string> responseQueue;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\n");
    tokenizer tokens(message, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
         tok_iter != tokens.end(); ++tok_iter) {

      std::string m = *tok_iter;

      // Characters to be transformed.
      std::map<char, std::string> transformations;
      transformations['&']  = std::string("&amp;");
      transformations['\''] = std::string("&apos;");
      transformations['"']  = std::string("&quot;");
      transformations['>']  = std::string("&gt;");
      transformations['<']  = std::string("&lt;");
      transformations[' ']  = std::string("&nbsp;");

      // Build list of characters to be searched for.
      std::string reserved_chars;
      for (auto ti = transformations.begin(); ti != transformations.end(); ++ti) {
        reserved_chars += ti->first;
      }

      size_t pos = 0;
      while (std::string::npos != (pos = m.find_first_of(reserved_chars, pos))) {
        m.replace(pos, 1, transformations[m[pos]]);
        ++pos;
      }

      responseQueue.push(m);
    }
    return responseQueue;
  }


  void Game::closeNetworkConnection(network::ConnectionId id) {
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

    GameTimer timer(.05f, 0);
    timer.registerInterupt(boost::bind(&Game::inShutdown, this));
    timer.registerCallback(0.001f,
                           boost::bind(&omush::Game::handleNewConnections,
                                       this));

    timer.registerCallback(0.001f,
                           boost::bind(&omush::Game::handleIncommingMessages,
                                       this));

    server_->start();
    timer.run();
    shutdown();

    delete server_;
  }

  void Game::shutdown() {


    shutdown_ = true;
    // Loop through and boot everyone.
  }

  /**
   * Handle SIGINT by setting the shutdown flag on the game.
   *
   * @param signum Signal caught by SignalHandler.
   */
  void Game::handleSignal(int signum) {
    shutdown();
  }

}  // namespace omush
