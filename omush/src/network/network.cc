/// Copyright 2014 Michael Smith

#include "omush/network/network.h"
#include "omush/network/descriptor.h"
#include "omush/network/queue.h"
#include "websocketpp/server.hpp"


namespace omush {
  namespace network {
    Network::Network() {
      outputQueue_ = new OutputQueue();
      server_.init_asio();
      server_.set_open_handler(bind(&Network::onOpen, this, ::_1));
      server_.set_close_handler(bind(&Network::onClose, this, ::_1));
      server_.set_message_handler(bind(&Network::onMessage, this, ::_1, ::_2));
    }

    Network::~Network() {
    }

    void Network::onMessage(websocketpp::connection_hdl hdl,
                            WSServer::message_ptr msg) {
      Descriptor* desc = getDescriptorFromHdl(hdl);
      desc->receivedMessage(msg);
    }

    Descriptor* Network::getDescriptorFromHdl(websocketpp::connection_hdl hdl) {
        auto it = connectedDescriptors_.find(hdl);

        if (it == connectedDescriptors_.end()) {
            // this connection is not in the list. This really shouldn't happen
            // and probably means something else is wrong.
            throw std::invalid_argument("No data avaliable for session");
        }

        return it->second;
    }

    void Network::onOpen(websocketpp::connection_hdl hdl) {
      std::cout << "Opened" << std::endl;

      Descriptor *desc = new Descriptor(hdl,
                                        environment_,
                                        inputQueue_,
                                        outputQueue_);
      connectedDescriptors_[hdl] = desc;
    }

    void Network::onClose(websocketpp::connection_hdl hdl) {
      delete connectedDescriptors_[hdl];
      connectedDescriptors_.erase(hdl);
      server_.close(hdl, websocketpp::close::status::blank,
                    "Shutdown");
    }

    void Network::listen(int port) {
      server_.listen(port);
      server_.start_accept();
    }

    void Network::shutdown() {
      server_.stop_listening();
      std::map<websocketpp::connection_hdl, Descriptor*>::iterator it;
      for (it = connectedDescriptors_.begin();
           it != connectedDescriptors_.end();
           ++it) {
        server_.close(it->first, websocketpp::close::status::blank,
              "Shutdown");

        delete it->second;
        connectedDescriptors_.erase(it->first);
      }
    }

    void Network::poll() {
      server_.poll();
    }

    void Network::push() {
      while (!outputQueue_->queue_.empty()) {
        OutputMessage msg = outputQueue_->queue_.front();
        outputQueue_->queue_.pop();
        server_.send(msg.d->getHdl(),
                     msg.message,
                     websocketpp::frame::opcode::value::TEXT);
      }
    }

    void Network::setupQueues(InputQueue& input) {
      this->inputQueue_ = &input;
    }

    void Network::setupEnvironment(Environment& env) {
      this->environment_ = &env;
    }

  }  // namespace network
}  // namespace omush
