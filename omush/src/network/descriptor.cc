///
/// descriptor.cc
///
/// Copyright 2014 Michael Smith
///

#include "omush/network/descriptor.h"
#include "omush/network/queue.h"
#include "omush/command/command.h"

namespace omush {
  namespace network {

    Descriptor::Descriptor(InputQueue* input) {
      this->inputQueue_ = input;

      // TODO(msmith): Move this into an object variable so it's accessabel from
      // receivedMessage.
      command::CommandParser p;
      p.registerCommand("CommandQuit");
      p.registerCommand("CommandWho");

      p.run("who");
    }

    Descriptor::~Descriptor() {
    }

    void Descriptor::receivedMessage(WSServer::message_ptr msg) {
      // Do any handling with the message. We'll just pass it along to
      // input queue.
      //      inputQueue->.addMessage(msg.get_payload());
      // Run through the global command parser.
      // Run through the connection screen command parser.

      // Send it back to the input buffer to run through the connected parser.

      // If I'm connected to something, deal with it.
    }

  }  // namespace network
}  // namespace omush


