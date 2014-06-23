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

  CommandInventory::CommandInventory() : ICommand("@INVENTORY") {
  }

  CommandInfo CommandInventory::process(CommandContext& context) {
    CommandInfo info;
    /*
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());

    if (cmdSplit.size() <= 1) {
      info.errorString = "Dig what?";
      return info;
    }
    info.rawArgs = cmdSplit[1];

    std::vector<std::string> eqSplit = splitStringIntoSegments(info.rawArgs, "=", 2);
    if (eqSplit.size() > 0) {
      std::vector<std::string> arg0;
      arg0.push_back(processExpression(eqSplit[0], context.funcScope).basicString());
      info.eqArgs.push_back(arg0);
    }

    if (eqSplit.size() > 1) {

      std::vector<std::string> arg0;
      std::string arg1  = processExpression(eqSplit[1], context.funcScope).basicString();
      std::vector<std::string> cSplit = splitStringIntoSegments(arg1, ",", 2);

      BOOST_FOREACH(std::string value, cSplit) {
        boost::trim(value);
        arg0.push_back(value);
      }

      info.eqArgs.push_back(arg0);
    }
    */
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
