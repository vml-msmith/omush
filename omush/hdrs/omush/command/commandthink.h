/**
 * \file commandthink.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDTHINK_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDTHINK_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandThink : public ICommand {
  public:
    CommandThink();
    bool run(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDTHINK_H_
