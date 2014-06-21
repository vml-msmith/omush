/**
 * \file commandlook.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDLOOK_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDLOOK_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandLook : public ICommand {
  public:
    CommandLook();
    bool run(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDLOOK_H_
