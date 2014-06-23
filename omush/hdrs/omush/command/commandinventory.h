/**
 * \file commandinventory.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDINVENTORY_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDINVENTORY_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandInventory : public ICommand {
  public:
    CommandInventory();
    bool run(CommandContext& context);
  private:

    CommandInfo process(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDINVENTORY_H_
