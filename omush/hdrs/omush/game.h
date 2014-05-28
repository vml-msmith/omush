/**
 * game.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_GAME_H_
#define OMUSH_HDRS_OMUSH_GAME_H_

#import "signalhandler.h"
#include <boost/uuid/uuid.hpp>
#include <queue>
#include <map>
#include <string>

namespace omush {
  class Game;
  namespace network {
    class NetworkService;
    typedef boost::uuids::uuid ConnectionId;
  }

  struct Client {
    std::string name;
    bool isConnected;
    Client() : isConnected(false) {}
  };

  /**
   * This is the main class that is the the game. Everything is handled from
   * here.
   */
  class Game : public SignalHandlerDelegate {
   public:
    Game();
    ~Game();
    void run();

    virtual void handleSignal(int signal);
    bool inShutdown();
    bool handleNewConnections();
    bool handleIncommingMessages();
    void sendNetworkMessage(network::ConnectionId id, std::string message);
    void closeNetworkConnection(network::ConnectionId id);
    void shutdown();

   protected:
    std::queue<std::string> encodeString(std::string message);
    std::map<network::ConnectionId, Client> clientList_;
    network::NetworkService *server_;
    bool shutdown_;

    Game(const Game&);
    void operator=(const Game&);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_GAME_H_
