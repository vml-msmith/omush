/**
 * \file commandconnect.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_

#include <string>
#include "omush/command/icommand.h"

namespace omush {

  /**
   * Command to connect to a player from the welcome screen.
   *
   * Usage:
   *   connect <username> <password>
   *
   * The password can have no spaces. Any argument previous to the last argument
   * will be part of the name.
   *
   * Username can be the player's name (object @name), an Alias for the player
   * or the dbref of the player.
   */
  class CommandConnect : public ICommand {
   public:
    CommandConnect();
    bool run(CommandContext& context);
    CommandInfo process(CommandContext& context);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_
