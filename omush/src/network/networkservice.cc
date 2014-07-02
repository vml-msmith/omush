/**
 * \file networkservice.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/network/networkservice.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

namespace omush {
  namespace network {
    NetworkService::NetworkService(NetworkServiceConfig options)
      : config_(options), isRunning_(false) {
    }

    NetworkService::~NetworkService() {
      ConnectionList::iterator it;
      for (it = descriptors_.begin();
           it != descriptors_.end();
           ++it) {
        websocketpp::lib::error_code ec;
        server_.close(it->second.hdl,
                      websocketpp::close::status::going_away,
                      "",
                      ec);
      }
      server_.stop_listening();
    }

    void NetworkService::start() {
      if (isRunning_ == true) {
        return;
      }

      server_.init_asio();
      server_.set_open_handler(bind(&NetworkService::onOpen,
                                    this,
                                    ::_1));
      server_.set_close_handler(bind(&NetworkService::onClose,
                                     this,
                                     ::_1));
      server_.set_message_handler(bind(&NetworkService::onMessage,
                                       this,
                                       ::_1,
                                       ::_2));

      server_.listen(config_.port);
      server_.start_accept();
      isRunning_ = true;
    }

    bool NetworkService::poll() {
      if (isRunning_ == true) {
        server_.poll();
      }

      server_.poll();
      return false;
    }

    void NetworkService::onMessage(ConnectionHdl hdl,
                            WSServer::message_ptr msg) {
      IncommingMessage iMsg;
      iMsg.id = hdlToConnectionId(hdl);
      iMsg.rawString = msg->get_payload();
      boost::trim_if(iMsg.rawString, boost::is_any_of(" \n"));

      incommingMessages_.push(iMsg);
    }

    void NetworkService::onOpen(ConnectionHdl hdl) {
      NetworkDescriptor desc = createDescriptor(hdl);
      descriptors_[desc.uniqueId] = desc;
    }

    void NetworkService::onClose(ConnectionHdl hdl) {
      std::cout << "Closed." << std::endl;;
    }


    std::vector<ConnectionId> NetworkService::connections() {
      std::vector<ConnectionId> keys;
      // Retrieve all keys
      for (ConnectionList::iterator iter = descriptors_.begin();
           iter != descriptors_.end();
           ++iter) {
        keys.push_back(iter->first);
      }

      return keys;
    }


    NetworkDescriptor NetworkService::createDescriptor(ConnectionHdl hdl) {
      NetworkDescriptor desc;
      desc.hdl = hdl;
      desc.uniqueId = boost::uuids::random_generator()();
      hdlToConnectionIdList_.insert(std::make_pair(hdl, desc.uniqueId));

      return desc;
    }

    ConnectionId NetworkService::hdlToConnectionId(ConnectionHdl hdl) {
      if (hdlToConnectionIdList_.find(hdl) == hdlToConnectionIdList_.end()) {
        //  hdlToUuidList_.insert(std::make_pair<hdl, boost::uuids::random_generator()()>);
      }
      return hdlToConnectionIdList_[hdl];
    }

    ConnectionHdl* NetworkService::ConnectionIdToHdl(ConnectionId id) {
      if (descriptors_.find(id) == descriptors_.end()) {
        return NULL;
      }

      return &(descriptors_[id].hdl);
    }

    void NetworkService::pushMessage(NetworkMessage msg) {
      ConnectionHdl hdl = *(ConnectionIdToHdl(msg.id));
      server_.send(hdl,
                   msg.rawString,
                   websocketpp::frame::opcode::value::TEXT);
    }


    IncommingMessage NetworkService::popIncommingMessage() {
      IncommingMessage msg = incommingMessages_.front();
      incommingMessages_.pop();
      return msg;
    }

    unsigned int NetworkService::getIncommingMessageCount() {
      return incommingMessages_.size();
    }

    void NetworkService::closeNetworkConnection(ConnectionId id) {
      server_.close(*(ConnectionIdToHdl(id)), websocketpp::close::status::blank, "Shutdown");
    }
  }  // namespace network
}  // namespace omush
