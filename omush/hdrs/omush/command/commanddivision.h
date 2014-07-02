/**
 * \file commanddivision.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIVISION_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIVISION_H_

#include "omush/command/icommand.h"

namespace omush {

  class CommandDivision : public ICommand {
   public:
    CommandDivision();
    bool run(CommandContext& context);

   private:
    bool create(CommandContext& context, CommandInfo& info);
    /*
    bool runAdd(CommandContext& context, CommandInfo& info);
    bool runRemove(CommandContext& context, CommandInfo& info);
    */
    CommandInfo process(CommandContext& context);
  };

} // omush

#endif //  OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIVISION_H_
