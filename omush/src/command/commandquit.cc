/**
 * \file commandquit.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandquit.h"

namespace omush {
  CommandQuit::CommandQuit() {
    name_ = "QUIT";
    absolute_ = true;
  }

  bool CommandQuit::run(std::string calledAs, std::string input, CommandContext context) {
    Command::run_(calledAs, input, context);
    context.game->sendNetworkMessage(context.descriptor, "Goodbye.");
    context.game->closeNetworkConnection(context.descriptor);
    return true;
  }
}  // namespace omush
