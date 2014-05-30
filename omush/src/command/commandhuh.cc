/**
 * \file commandhuh.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandhuh.h"
//#include <boost/algorithm/string.hpp>
#include "omush/database.h"
#include "omush/action.h"

namespace omush {
  CommandHuh::CommandHuh() {
    name_ = "HUH";
    absolute_ = false;
  }

  bool CommandHuh::run(std::string calledAs, std::string input, CommandContext context) {
    std::cout << "Huh: " << context.ref << std::endl;

    ActionHuh(context.db, context.game, context.db->findObjectByDbref(context.ref)).enact();
    return true;
  }

}  // namespace omush