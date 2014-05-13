///
/// descriptor.cc
///
/// Copyright 2014 Michael Smith
///

#include "omush/network/descriptor.h"
#include "omush/network/queue.h"
#include "omush/command/command.h"
#include "omush/environment.h"
#include "omush/database/database.h"

namespace omush {
  namespace network {

    Descriptor::Descriptor(websocketpp::connection_hdl hdl,
                           Environment* env,
                           InputQueue* input,
                           OutputQueue* output) {
      this->inputQueue_ = input;
      this->outputQueue_ = output;
      this->environment_ = env;
      this->player_ = NULL;
      this->hdl_ = hdl;

      sendWelcomeScreen_();
    }

    Descriptor::~Descriptor() {
    }

    void Descriptor::receivedMessage(WSServer::message_ptr msg) {
      if (environment_->descriptorCommands.run(this,
                                               NULL,
                                               msg->get_payload())) {
        return;
      }
      std::cout << "Trt welcom" << std::endl;
      if (player_ == NULL &&
          environment_->welcomeCommands.run(this,
                                            NULL,
                                            msg->get_payload())) {
        return;
      }
      std::cout << "add to queue" << std::endl;
      if (player_ == NULL) {
        // Something like.. welcome screen?
        return;
      }

      // Now add it.
      inputQueue_->addMessage(reinterpret_cast<database::DatabaseObject*>(player_), msg->get_payload());


      // Do any handling with the message. We'll just pass it along to
      // input queue.
      //      inputQueue->.addMessage(msg.get_payload());
      // Run through the global command parser.
      // Run through the connection screen command parser.

      // Send it back to the input buffer to run through the connected parser.

      // If I'm connected to something, deal with it.
    }

    void Descriptor::doLogin(database::Player* p) {
      player_ = p;
    }

    void Descriptor::sendMessage(std::string message) {
      outputQueue_->addByDescriptor(this, message);
    }

    void Descriptor::sendWelcomeScreen_() {
      std::string welcome = "--------------------------------------------------------------------------------\n                                  Welcome\n--------------------------------------------------------------------------------\n\nCommands:\n  connect <your name> <your password>\n  create <new name> <new password>\n  help\n  quit";
      sendMessage(welcome);
    }

    websocketpp::connection_hdl Descriptor::getHdl() {
      return hdl_;
    }

    void OutputQueue::addByDescriptor(Descriptor *d, std::string message) {
      OutputMessage msg;
      msg.message = message;
      msg.d = d;

      queue_.push(msg);
    }


  }  // namespace network
}  // namespace omush
