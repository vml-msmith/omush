/**
 * \file commandset.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_

#include "omush/command/command.h"
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
    void notify(CommandContext context,
                database::DatabaseObject *enactor,
                std::string msg);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDSET_H_
