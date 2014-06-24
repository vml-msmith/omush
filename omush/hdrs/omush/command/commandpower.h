/**
 * \file commandpower.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDPOWER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDPOWER_H_

#include "omush/command/icommand.h"

namespace omush {

  class CommandPower : public ICommand {
   public:
    CommandPower();
    bool run(CommandContext& context);

   private:
    bool runAdd(CommandContext& context, CommandInfo& info);
    bool runRemove(CommandContext& context, CommandInfo& info);
    CommandInfo process(CommandContext& context);
  };

} // omush

#endif //  OMUSH_HDRS_OMUSH_COMMAND_COMMANDPOWER_H_
