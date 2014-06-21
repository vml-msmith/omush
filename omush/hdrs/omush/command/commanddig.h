/**
 * \file commanddig.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_

#include "omush/command/icommand.h"
#include <string>

namespace omush {
  namespace database {
    class DatabaseObject;
  }

  struct CommandInfo {
    std::vector<std::vector<std::string> > eqArgs;
    std::vector<std::string> switches;
    std::string errorString;
    std::string rawArgs;

  CommandInfo() : errorString(""), rawArgs("") {}
  };

  class CommandDig : public ICommand {
  public:
    CommandDig();
    bool run(CommandContext& context);
  private:
    CommandInfo process(CommandContext& context);

    void notify(CommandContext context,
                database::DatabaseObject *enactor,
                std::string msg);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_
