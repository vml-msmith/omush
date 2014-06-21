/**
 * \file commandsay.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDSAY_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDSAY_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {

  class CommandSay : public ICommand {
  public:
    CommandSay();
    bool run(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDSAY_H_
