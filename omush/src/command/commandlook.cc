/**
 * \file commandlook.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandlook.h"
//#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action/action.h"

namespace omush {
  CommandLook::CommandLook() {
    name_ = "LOOK";
    absolute_ = false;
  }

  bool CommandLook::run(std::string calledAs, std::string input, CommandContext context) {
    ActionLook(context.db, context.game, context.db->findObjectByDbref(context.dbref)).enact();
    return true;
  }

}  // namespace omush
