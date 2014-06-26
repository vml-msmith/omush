/**
 * \file commandinventory.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commandinventory.h"
#include "omush/action/actioninventory.h"
#include "omush/function/function.h"
#include "omush/database/helpers.h"

namespace omush {

  CommandInventory::CommandInventory() : ICommand("INVENTORY") {
  }

  CommandInfo CommandInventory::process(CommandContext& context) {
    CommandInfo info;
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    if (cmdSplit.size() > 1) {
      info.errorString = "Inventory takes no arguments.";
      return info;
    }

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());

    if (info.switches.size() > 0) {
      info.errorString = "Inventory takes no switches.";
      return info;
    }

    return info;
  }

  bool CommandInventory::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    ActionInventory action(context);
    action.enact();
    return true;

  }

}  // namespace omush
