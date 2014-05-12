///
/// Descriptor.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_NETWORK_DESCRIPTOR_H_
#define OMUSH_HDRS_OMUSH_NETWORK_DESCRIPTOR_H_

#include "omush/network/queue.h"
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace omush {
  namespace network {
    typedef websocketpp::server<websocketpp::config::asio> WSServer;

    class Descriptor {
     public:
      explicit Descriptor(InputQueue* input);
      void receivedMessage(WSServer::message_ptr msg);
      ~Descriptor();

     private:
      InputQueue* inputQueue_;
    };
  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_DESCRIPTOR_H_
