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
  class Environment;
  namespace database {
    class Player;
  }
  namespace network {
    typedef websocketpp::server<websocketpp::config::asio> WSServer;

    class Descriptor {
     public:
      explicit Descriptor(websocketpp::connection_hdl hdl, Environment* env, InputQueue* input, OutputQueue* output);
      void receivedMessage(WSServer::message_ptr msg);
      ~Descriptor();

      void doLogin(database::Player* p);
      void sendMessage(std::string message);
      websocketpp::connection_hdl getHdl();
     private:
      void sendWelcomeScreen_();

      InputQueue* inputQueue_;
      OutputQueue* outputQueue_;
      Environment * environment_;
      database::Player *player_;
      websocketpp::connection_hdl hdl_;
    };
  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_DESCRIPTOR_H_
