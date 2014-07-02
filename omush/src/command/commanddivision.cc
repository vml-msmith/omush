/**
 * \file commanddivision.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commanddivision.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/utility.h"
#include "omush/function/function.h"
#include "omush/database/targetmatcher.h"
#include "omush/action/actioncreatedivision.h"
#include "omush/action/actiondivisionattach.h"

namespace omush {
  CommandDivision::CommandDivision() : ICommand("@DIVISION") {
  }

  bool CommandDivision::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    if (info.switches.size() > 0) {
      if (boost::iequals(info.switches[0], "CREATE")) {
        return create(context, info);
      } else {
        notifyExecutor(context, "Unrecognized swich. This is a bug.");
      }
    }

    if (info.eqArgs.size() != 2) {
      notifyExecutor(context, "@DIVISION requires two arguments (@division <player>=<division>).");
      return true;
    }

    database::DatabaseObject* player = context.cmdScope.executor;
    using namespace database;
    std::vector<DatabaseObject*> matches;
    TargetMatcher matcher = TargetMatcher(context.db,
                                          context.cmdScope.executor);

    matches = matcher.match(info.eqArgs[0][0]);
    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know what you want to attach.");
      return true;
    }
    if (matches.size() == 0) {
      notifyExecutor(context, "I don't see that here.");
      return true;
    }
    player = matches[0];

    matcher.type(database::DbObjectTypeDivision);
    matches = matcher.match(info.eqArgs[1][0]);
    if (matches.size() > 1) {
      notifyExecutor(context, "I don't know what division you mean.");
      return true;
    }
    if (matches.size() == 0) {
      notifyExecutor(context, "I don't see that division.");
      return true;
    }

    ActionDivisionAttach(context).target(player).division(matches[0]).enact();
    return true;
  }

  bool CommandDivision::create(CommandContext& context, CommandInfo& info) {
    if (info.eqArgs.size() == 0) {
      notifyExecutor(context, "You need to specify a name to create a division.");
      return true;
    }

    if (info.eqArgs.size() > 1) {
      notifyExecutor(context, "@DIVISION/CREATE takes only one argument.");
      return true;
    }

    ActionCreateDivision create(context);
    create.name(info.eqArgs[0][0]);
    create.enact();
    return true;
  }

  CommandInfo CommandDivision::process(CommandContext& context) {
    CommandInfo info;

    // TODO(msmith): Make a method to split out switches.
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());
    std::map<std::string, bool> possibleSwitches;

    possibleSwitches["CREATE"] = 1;

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
