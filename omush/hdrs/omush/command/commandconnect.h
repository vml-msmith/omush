/**
 * \file commandconnect.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandConnect : public ICommand {
  public:
    CommandConnect();
    bool run(CommandContext& context);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONNECT_H_
