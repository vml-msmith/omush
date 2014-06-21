/**
 * \file commandquit.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDQUIT_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDQUIT_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandQuit : public ICommand {
  public:
    CommandQuit();
    bool run(CommandContext& context);
    CommandQuit* factory();
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDQUIT_H_
