/**
 * \file commanddig.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_

#include "omush/command/command.h"
#include <string>

namespace omush {
  namespace database {
    class DatabaseObject;
  }

  class CommandDig : public ICommand {
  public:
    CommandDig();
    bool run(std::string calledAs, std::string input, CommandContext context);
  private:
    void notify(CommandContext context,
                database::DatabaseObject *enactor,
                std::string msg);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDDIG_H_
