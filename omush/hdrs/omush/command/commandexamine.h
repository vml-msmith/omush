/**
 * \file commandexamine.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDEXAMINE_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDEXAMINE_H_

#include "omush/command/command.h"
#include <string>

namespace omush {
  class CommandExamine : public ICommand {
  public:
    CommandExamine();
    bool run(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDEXAMINE_H_
