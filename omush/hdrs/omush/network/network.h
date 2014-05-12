///
/// network.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_

#include <map>

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

#include "omush/network/queue.h"


namespace omush {
  namespace network {

    extern class Descriptor;

    typedef websocketpp::server<websocketpp::config::asio> WSServer;
    ///
    ///
    class Network {
     public:
      Network();
      ~Network();

      void onMessage(websocketpp::connection_hdl hdl,
                     WSServer::message_ptr msg);
      void onOpen(websocketpp::connection_hdl hdl);
      void onClose(websocketpp::connection_hdl hdl);

      void listen(int port);
      void shutdown();
      void poll();
      void setupQueues(InputQueue& input);

     private:
      Descriptor* getDescriptorFromHdl(websocketpp::connection_hdl hdl);

      WSServer server_;
      typedef std::map<websocketpp::connection_hdl,
        Descriptor*,
        std::owner_less<websocketpp::connection_hdl> > con_list;

      con_list connectedDescriptors_;

      InputQueue *inputQueue_;

      Network(const Network&);
      void operator=(const Network&);
    };
  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_NETWORK_H_
