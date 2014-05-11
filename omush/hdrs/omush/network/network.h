///
/// network.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace omush {
  namespace network {

    typedef websocketpp::server<websocketpp::config::asio> WSServer;
    ///
    ///
    class Network {
     public:
      Network();
      ~Network();

      void onMessage(websocketpp::connection_hdl hdl, WSServer::message_ptr msg);
      void onOpen(websocketpp::connection_hdl hdl);
      void onClose(websocketpp::connection_hdl hdl);

      void listen(int port);
      void shutdown();
      void poll();

     private:
      WSServer server_;
      Network(const Network&);
      void operator=(const Network&);
    };
  }
}  // namespace omush::network

#endif  // OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_
