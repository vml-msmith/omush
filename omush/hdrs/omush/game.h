/**
 * \file game.h
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
  namespace database {
    class Database;
  };

  namespace network {
    class NetworkService;
    typedef boost::uuids::uuid ConnectionId;
  }
  namespace database {
    typedef long Dbref;
  }

  struct InternalCommand {
    std::string cmd;
    database::Dbref dbref;
    InternalCommand(database::Dbref r, std::string c) : cmd(c), dbref(r) {}
  };

  struct Client {
    std::string name;
    bool isConnected;
    database::Dbref dbref;

    Client() : isConnected(false) {}
  };


  typedef std::map<network::ConnectionId, Client> ClientMap;

  /**
   * This is the main class that is the the game. Everything is handled from
   * here.
   */
  class Game : public SignalHandlerDelegate {
   public:
    /**
     * Constructor. Do any initial resetting or setup of theg game that doesn't
     * require on external configuration.
     */
    Game();

    /**
     * Destructor. Free up any resources that were allocated during the lifetime
     *of the game.
     */
    ~Game();

    /**
     * The main loop of the program. When this method returns, the program
     * should end.
     */
    void run();

    /**
     * Handle SIGINT by setting the shutdown flag on the game.
     *
     * @param signum Signal caught by SignalHandler.
     */
    virtual void handleSignal(int signal);

    /**
     * Return if game should be in shutdown cycle.
     *
     * @return true if the game should be shutdown (stop looping)
     * @return false if the game should still keep going.
     */
    bool inShutdown();

    /**
     * Retrive and process new connections from the network layer.
     */
    bool handleNewConnections();

    /**
     * Retrive and process new messages from the network layer.
     *
     * Messages that are from connected users (those past the connect screen)
     * are added to an internal message queue and processed at a later time. The
     * it's done this way, is that objects in the game which are not connected
     * as network objects should still be able to execute commands with the same
     * priority as connected clients. Thus these messages are all handled at the
     * same time.
     */
    bool handleIncommingMessages();

    bool processCommands();

    /**
     * Send a string message to a connected descriptor.
     */
    void sendNetworkMessage(network::ConnectionId id, std::string message);

    /**
     * Close a connected descriptor.
     */
    void closeNetworkConnection(network::ConnectionId id);

    /**
     * Shutdown the game.
     */
    void shutdown();


    ClientMap& clientList() {
      return clientList_;
    }
   protected:
    std::queue<InternalCommand> commandList_;
    ClientMap clientList_;
    network::NetworkService *server_;
    bool shutdown_;
    database::Database *db_;

    Game(const Game&);
    void operator=(const Game&);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_GAME_H_
