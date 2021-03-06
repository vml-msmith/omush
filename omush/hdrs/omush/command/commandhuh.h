/**
 * \file commandhuh.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDHUH_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDHUH_H_

#include "omush/command/command.h"
#include <string>

namespace omush {
  class CommandHuh : public Command {
  public:
    CommandHuh();
    bool run(std::string calledAs, std::string input, CommandContext context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDHUH_H_
