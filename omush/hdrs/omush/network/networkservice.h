/**
 * \file networkservice.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICE_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICE_H_

#include <boost/uuid/uuid.hpp>
#include <map>
#include <vector>
#include <queue>

#include "omush/network/networkdescriptor.h"
#include "omush/network/networkserviceconfig.h"
#include "omush/network/networkmessage.h"

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace omush {
  namespace network {
    /**
     * Server object from websocketpp;
     */
    typedef websocketpp::server<websocketpp::config::asio> WSServer;

    /**
     * Unique ID for a connected descriptor.
     */
    typedef boost::uuids::uuid ConnectionId;

    /**
     * ConnectionId to NetworkDescriptor map.
     */
    typedef std::map<ConnectionId, NetworkDescriptor> ConnectionList;

    /**
     * Alias NetworkMessage to the more specific IncommingMessage.
     */
    typedef NetworkMessage IncommingMessage;

    /**
     * Connection handler for websocketpp.
     */
    typedef websocketpp::connection_hdl ConnectionHdl;

    /**
     * Public interface to the Network connection.
     *
     * This class will handle all the backend implementation of the networking
     * code and allows the owner of the object to retrieve connections
     * as a unique ID (from boost::uuid). Messages may also be retrieved or
     * sent via the same unique ID.
     */
    class NetworkService {
     public:
      /**
       * Default constructor. Takes an configuration argument.
       *
       * Initializies the configuration and sets isRunning to FALSE.
       *
       * @param Options object, containing information pertaining to the type
       *  of connection.
       */
      explicit NetworkService(NetworkServiceConfig options);

      /**
       * Default destructor. Will close all open connections and shutdown
       * any underlying services.
       */
      ~NetworkService();

      /**
       * Initalize underlying network services, start listening for connections.
       *
       * This method will only work once. Any subsequent call to start() will
       * result in no action being taken.
       */
      void start();

      /**
       * Run exactly one loop of the underlying network service. This should be
       * called at least once per game loop, prefarably at the begining of the
       * loop. It will trigger acceptance of pending connetions, handling of
       * incomming data (add that data to the message queue) and send any data
       * waiting to be sent.
       */
      bool poll();

      /**
       * Handler for incomming network messages. This is used internally, but
       * needs to be a public method in order for 3rd party libraries to see it.
       *
       * TODO (msmith): Move this into another class to hide it.
       */
      void onMessage(ConnectionHdl hdl,
                     WSServer::message_ptr msg);

      /**
       * Handler for new network connections. This is used internally, but
       * needs to be a public method in order for 3rd party libraries to see it.
       *
       * TODO (msmith): Move this into another class to hide it.
       */
      void onOpen(ConnectionHdl hdl);

      /**
       * Handler for closed network  connections. This is used internally, but
       * needs to be a public method in order for 3rd party libraries to see it.
       *
       * TODO (msmith): Move this into another class to hide it.
       */
      void onClose(ConnectionHdl hdl);

      /**
       * Get a list of connections.
       */
      std::vector<ConnectionId> connections();

      /**
       * Send a message across the network to a client.
       *
       * @param A NetworkMessage object that contains the rawText as well as
       *        the ConnetionId of the connection to send the message to.
       */
      void pushMessage(NetworkMessage msg);

      /**
       * Retrieve the next incomming message currently stored in queue, and pop
       * that message out of the queue.
       *
       * @return IncommingMessge object that contains the rawTet as well as a
       *         unique ConnectionId of the sending client.
       */
      IncommingMessage popIncommingMessage();

      /**
       * Get the number of messages left in the incomming message queue.
       */
      unsigned int getIncommingMessageCount();

      /**
       * Drop a client connection and remove it's handler from the list of
       * connected clients.
       */
      void closeNetworkConnection(ConnectionId id);

     private:
      ConnectionId hdlToConnectionId(ConnectionHdl hdl);
      ConnectionHdl* ConnectionIdToHdl(ConnectionId id);

      NetworkDescriptor createDescriptor(ConnectionHdl hdl);

      bool isRunning_;
      NetworkServiceConfig config_;
      WSServer server_;

      ConnectionList descriptors_;

      std::map<ConnectionHdl,
        ConnectionId,
        std::owner_less<ConnectionHdl> > hdlToConnectionIdList_;


      std::queue<IncommingMessage> incommingMessages_;
      NetworkService(const NetworkService&);
      void operator=(const NetworkService&);
    };

  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICE_H_
