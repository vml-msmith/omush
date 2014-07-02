/**
 * \file commanddrop.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commanddrop.h"
#include "omush/database/targetmatcher.h"
#include "omush/function/function.h"
#include "omush/database/targetmatcher.h"
#include "omush/action/actiondrop.h"

namespace omush {
  CommandDrop::CommandDrop() : ICommand("DROP") {
  }

  CommandInfo CommandDrop::process(CommandContext& context) {
    CommandInfo info;
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());


    std::map<std::string, bool> possibleSwitches;

    BOOST_FOREACH(std::string key, info.switches) {
      boost::to_upper(key);
      if (possibleSwitches.find(key) == possibleSwitches.end()) {
        info.errorString = "Unrecognized switch: " + key;
        return info;
      }
    }

    if (cmdSplit.size() < 2) {
      info.rawArgs = "";
      return info;
    }
    info.rawArgs = cmdSplit[1];

    std::vector<std::string> arg0;
    arg0.push_back(processExpression(info.rawArgs, context.funcScope).basicString());
    info.eqArgs.push_back(arg0);

    return info;
  }

  bool CommandDrop::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    if (info.eqArgs.size() == 0) {
      notifyExecutor(context, "I don't know what you want to drop.");
      return true;
    }

    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher tm(context.db, context.cmdScope.executor);
    tm.setSearchFlags(SEARCH_INVENTORY);
    matches = tm.match(info.eqArgs[0][0]);

    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know which one you mean.");
      return true;
    }
    if (matches.size() == 0) {
      notifyExecutor(context, "You don't have that.");
      return true;
    }

    ActionDrop(context).target(matches[0]).enact();
  }

}  // namespace omush
