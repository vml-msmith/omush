/**
 * \file commandset.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commandset.h"
#include <boost/algorithm/string.hpp>

#include "omush/utility.h"
#include "omush/database/database.h"
#include "omush/action/actionsetattribute.h"
#include "omush/action/actionsetflag.h"
#include "omush/database/targetmatcher.h"
#include "omush/function/function.h"

namespace omush {
  CommandSet::CommandSet() : ICommand("@SET") {
  }

  CommandInfo CommandSet::process(CommandContext& context) {
    CommandInfo info;
    bool noeval = false;

    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());

    std::map<std::string, bool> possibleSwitches;
    possibleSwitches["NOEVAL"] = 1;

    BOOST_FOREACH(std::string key, info.switches) {
      boost::to_upper(key);
      if (possibleSwitches.find(key) == possibleSwitches.end()) {
        info.errorString = "Unrecognized switch: " + key;
        return info;
      }

      if (key.compare("NOEVAL") == 0)
        noeval = true;
    }

    if (cmdSplit.size() == 1)
      return info;

    info.rawArgs = cmdSplit[1];

    std::vector<std::string> eqSplit = splitStringIntoSegments(info.rawArgs, "=", 2);
    BOOST_FOREACH(std::string key, eqSplit) {
      std::vector<std::string> arg;
      std::string result = key;
      if (!noeval)
        result = processExpression(key, context.funcScope).basicString();
      arg.push_back(result);
      info.eqArgs.push_back(arg);
    }

    if (info.eqArgs.size() > 1) {
      // Handle ":"
      std::vector<std::string> colonSplit = splitStringIntoSegments(info.eqArgs[1][0], ":", 2);
      if (colonSplit[0].length() > 0) {
        std::vector<std::string> spaceSplit = splitStringIntoSegments(colonSplit[0], " ", 2);
        if (spaceSplit.size() == 1) {
          info.eqArgs[1][0] = colonSplit[0];
          if (colonSplit.size() > 1) {
            info.eqArgs[1].push_back(colonSplit[1]);
          }
        }
      }
    }
    return info;
  }


  bool CommandSet::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    if (info.eqArgs.size() != 2) {
      notifyExecutor(context, "@SET requires two arguments.");
      return true;
    }

    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher = database::TargetMatcher(context.db,
                                                              context.cmdScope.executor);
    matches = matcher.match(info.eqArgs[0][0]);

    if (matches.size() == 0) {
      notifyExecutor(context, "I can't see that here.");
      return true;
    }

    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know which one you mean.");
      return true;
    }

    if (info.eqArgs[1].size() == 1) {
      return setFlag(context, info, matches[0]);
    }

    return setAttribute(context, info, matches[0]);
  }

  bool CommandSet::setAttribute(CommandContext& context,
                                CommandInfo &info,
                                database::DatabaseObject* target) {
    ActionSetAttribute(context)
      .target(target)
      .attribute(info.eqArgs[1][0])
      .value(info.eqArgs[1][1])
      .enact();
    return true;
  }

  bool CommandSet::setFlag(CommandContext& context,
                           CommandInfo &info,
                           database::DatabaseObject* target) {
    bool unset = false;
    std::string flagName = info.eqArgs[1][0];
    const char *firstChar = flagName.c_str();

    if (*firstChar == '!' && flagName.length() > 1) {
      unset = true;
      flagName = flagName.substr(1, flagName.length());
    }

    Flag* flag = context.db->flags.getFlag(flagName);
    if (flag == NULL) {
      notifyExecutor(context, "I don't know that flag.");
      return true;
    }
    ActionSetFlag action = ActionSetFlag(context);
    action.target(target);
    action.flag(flag);

    if (unset) {
      action.unset();
    }

    action.enact();

    return true;
  }

}  // namespace omush
