/**
 * \file commandpower.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandpower.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/utility.h"
#include "omush/function/function.h"
#include "omush/database/targetmatcher.h"
#include "omush/action/actionpowerlist.h"
#include "omush/action/actionpoweradd.h"
#include "omush/action/actionpowerremove.h"

namespace omush {
  CommandPower::CommandPower() : ICommand("@POWER") {
  }

  bool CommandPower::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    if (info.switches.size() > 0) {
      // Only two switches are allowed, add and remove. If we get
      // this far, it has to be one of the two. No need to check the
      // else.
      if (boost::iequals(info.switches[0], "ADD")) {
        return runAdd(context, info);
      } else {
        return runRemove(context, info);
      }
    }

    // Run the default, which is listing the powers.
    if (info.eqArgs.size() > 1) {
      notifyExecutor(context, "This command takes only one argument.");
      return true;
    }

    database::DatabaseObject* player = context.cmdScope.executor;
    if (info.eqArgs.size() > 0) {
      using namespace database;
      std::vector<DatabaseObject*> matches;
      TargetMatcher matcher = TargetMatcher(context.db,
                                            context.cmdScope.executor);
      matcher.type(database::DbObjectTypePlayer);
      matches = matcher.match(info.eqArgs[0][0]);
      std::cout << "Match... " << info.eqArgs[0][0] << std::endl;
      if (matches.size() > 1) {
        notifyExecutor(context, "I don't know which player you mean.");
        return true;
      }
      if (matches.size() == 0) {
        notifyExecutor(context, "I don't see that player here.");
        return true;
      }
      player = matches[0];
    }

    ActionPowerList(context).player(player).enact();
    return true;
  }

  bool CommandPower::runAdd(CommandContext& context, CommandInfo& info) {
    if (info.eqArgs.size() < 2) {
      notifyExecutor(context, "@power/add requires two arguments.");
      return true;
    }

    using namespace database;
    DatabaseObject* player = NULL;
    std::vector<DatabaseObject*> matches;
    TargetMatcher matcher = TargetMatcher(context.db,
                                            context.cmdScope.executor);

    matcher.type(database::DbObjectTypePlayer);
    matches = matcher.match(info.eqArgs[0][0]);
    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know which player you mean.");
      return true;
    }

    if (matches.size() == 0) {
      notifyExecutor(context, "I don't see that player here.");
      return true;
    }

    player = matches[0];
    Power* p = context.db->powers.getPower(info.eqArgs[1][0]);
    if (p == NULL) {
      notifyExecutor(context, "I don't recognize that power.");
      return true;
    }

    int level = context.db->powers.getLevelFromString(info.eqArgs[1][0]);
    std::cout << "LEveL " << level << std::endl;
    // TODO: Add power checking. I dunno if this should be in action,
    // or command.

    ActionPowerAdd(context).player(player).power(p).level(level).enact();
    return true;
  }

  bool CommandPower::runRemove(CommandContext& context, CommandInfo& info) {
    if (info.eqArgs.size() < 2) {
      notifyExecutor(context, "@power/remove requires two arguments.");
      return true;
    }

    using namespace database;
    DatabaseObject* player = NULL;
    std::vector<DatabaseObject*> matches;
    TargetMatcher matcher = TargetMatcher(context.db,
                                            context.cmdScope.executor);

    matcher.type(database::DbObjectTypePlayer);
    matches = matcher.match(info.eqArgs[0][0]);
    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know which player you mean.");
      return true;
    }

    if (matches.size() == 0) {
      notifyExecutor(context, "I don't see that player here.");
      return true;
    }

    player = matches[0];
    Power* p = context.db->powers.getPower(info.eqArgs[1][0]);
    if (p == NULL) {
      notifyExecutor(context, "I don't recognize that power.");
      return true;
    }

    // TODO: Add power checking. I dunno if this should be in action,
    // or command.

    ActionPowerRemove(context).player(player).power(p).enact();
    return true;
  }

  CommandInfo CommandPower::process(CommandContext& context) {
    CommandInfo info;

    // TODO(msmith): Make a method to split out switches.
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());
    std::map<std::string, bool> possibleSwitches;

    possibleSwitches["ADD"] = 1;
    possibleSwitches["REMOVE"] = 1;

    BOOST_FOREACH(std::string key, info.switches) {
      boost::to_upper(key);
      if (possibleSwitches.find(key) == possibleSwitches.end()) {
        info.errorString = "Unrecognized switch: " + key;
        return info;
      }
    }

    if (cmdSplit.size() == 1)
      return info;

    info.rawArgs = cmdSplit[1];

    std::vector<std::string> eqSplit = splitStringIntoSegments(info.rawArgs, "=", 2);
    BOOST_FOREACH(std::string key, eqSplit) {
      std::vector<std::string> arg;
      arg.push_back(processExpression(key, context.funcScope).basicString());
      info.eqArgs.push_back(arg);
    }

    return info;
  }
}  // namespace omush
