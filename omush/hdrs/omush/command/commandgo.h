/**
 * \file commandgo.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDGO_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDGO_H_

#include "omush/command/icommand.h"
#include <string>
#include <iostream>

namespace omush {
  class CommandGo : public ICommand {
  public:
    CommandGo();
    bool run(CommandContext& context);
    CommandGo* factory();
    ~CommandGo() { std::cout << "Here" << std::endl; }
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDGO_H_
