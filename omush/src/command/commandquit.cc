/**
 * \file commandquit.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandquit.h"
#include "omush/game.h"

namespace omush {
  CommandQuit::CommandQuit() : ICommand("QUIT") {
  }

  CommandQuit* CommandQuit::factory() {
    return new CommandQuit();
  }

  bool CommandQuit::run(CommandContext& context) {
    //    Command::run_(calledAs, input, context);
    context.game->sendNetworkMessage(context.descriptor, "Goodbye.");
    context.game->closeNetworkConnection(context.descriptor);
    return true;
  }
}  // namespace omush
