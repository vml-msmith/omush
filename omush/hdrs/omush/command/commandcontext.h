/**
 * \file game.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_

#include "omush/database/definitions.h"
#include <string>
#include "omush/network/networkservice.h"

namespace omush {
  class Game;
  class Client;
  class FunctionScope;

  struct CommandScope {
    database::DatabaseObject *executor;
    database::DatabaseObject *enactor;
    database::DatabaseObject *caller;

    std::string originalString;
    std::string currentString;

    CommandScope() : executor(NULL), enactor(NULL), caller(NULL) {}
  };

  struct CommandContext {
    Game *game;
    Client *client;
    database::Database *db;
    network::ConnectionId descriptor;
    CommandScope cmdScope;
    FunctionScope* funcScope;

    std::string modifiedInput;
    database::Dbref executor;
    database::Dbref enactor;
    database::Dbref caller;
  };

}  // namespace CommandContext

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_
