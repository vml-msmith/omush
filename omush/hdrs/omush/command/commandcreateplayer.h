/**
 * \file commandcreateplayer.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATEPLAYER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATEPLAYER_H_

#include <string>
#include "omush/command/icommand.h"

namespace omush {

  /**
   * Command to create and connect to a player from the welcome screen.
   *
   * Usage:
   *   create <username> <password>
   *
   * The password can have no spaces. Any argument previous to the last argument
   * will be part of the name.
   */
  class CommandCreatePlayer : public ICommand {
   public:
    CommandCreatePlayer();
    bool run(CommandContext& context);
    CommandInfo process(CommandContext& context);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATEPLAYER_H_
