/// Copyright 2014 Michael Smith

#include "omush/network/network.h"

#include "websocketpp/server.hpp"

namespace omush {
  namespace network {
    Network::Network() {
      server_.init_asio();
      server_.set_open_handler(bind(&Network::onOpen, this, ::_1));
      server_.set_close_handler(bind(&Network::onClose, this, ::_1));
      server_.set_message_handler(bind(&Network::onMessage, this, ::_1, ::_2));
    }

    Network::~Network() {
    }

    void Network::onMessage(websocketpp::connection_hdl hdl, 
                            WSServer::message_ptr msg) {
        std::cout << msg->get_payload() << std::endl;
    }

    void Network::onOpen(websocketpp::connection_hdl hdl) {
      std::cout << "Opened" << std::endl;
    }

    void Network::onClose(websocketpp::connection_hdl hdl) {
      std::cout << "Closed" << std::endl;
    }


    void Network::listen(int port) {
      server_.listen(port);
      server_.start_accept();
    }

    void Network::shutdown() {
      server_.stop_listening();
    }

    void Network::poll() {
      server_.poll();
    }
  }
}
