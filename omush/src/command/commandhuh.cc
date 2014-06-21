/**
 * \file commandhuh.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandhuh.h"
#include "omush/database/database.h"
#include "omush/action/actionhuh.h"

namespace omush {
  CommandHuh::CommandHuh() : ICommand("HUH") {
  }

  bool CommandHuh::run(CommandContext& context) {
    ActionHuh(context.db, context.game, context.db->findObjectByDbref(context.executor)).enact();
    return true;
  }

}  // namespace omush
