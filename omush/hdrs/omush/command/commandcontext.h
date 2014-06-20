/**
 * \file game.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_

#include "omush/game.h";
#include "omush/database/definitions.h"
#include <string>
#include "omush/network/networkservice.h"

namespace omush {
  class Game;
  class Client;

  struct CommandContext {
    Game *game;
    Client *client;
    database::Database *db;
    network::ConnectionId descriptor;
    std::string modifiedInput;

    database::Dbref executor;
    database::Dbref enactor;
    database::Dbref caller;
  };

}  // namespace CommandContext

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDCONTEXT_H_
