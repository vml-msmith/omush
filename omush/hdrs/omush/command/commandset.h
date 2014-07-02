/**
 * \file commandset.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  namespace database {
    class DatabaseObject;
  }

  class CommandSet : public ICommand {
  public:
    CommandSet();
    bool run(CommandContext& context);
  private:
    bool setFlag(CommandContext& context,
                 CommandInfo &info,
                 database::DatabaseObject* target);
    bool setAttribute(CommandContext& context,
                      CommandInfo &info,
                      database::DatabaseObject* target);
    CommandInfo process(CommandContext& context);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_
