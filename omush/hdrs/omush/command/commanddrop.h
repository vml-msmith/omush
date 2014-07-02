/**
 * \file commanddrop.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDDROP_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDDROP_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  class CommandDrop : public ICommand {
   public:
    CommandDrop();
    bool run(CommandContext& context);
   private:
    CommandInfo process(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDDROP_H_
