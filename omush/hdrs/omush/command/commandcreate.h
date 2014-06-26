/**
 * \file commandcreate.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATE_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATE_H_

#include "omush/command/icommand.h"

namespace omush {
  class CommandCreate : public ICommand {
  public:
    CommandCreate();
    bool run(CommandContext& context);
  private:

    CommandInfo process(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCREATE_H_
