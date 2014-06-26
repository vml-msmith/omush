/**
 * \file commandcreate.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commandcreate.h"
#include <boost/algorithm/string.hpp>
#include "omush/action/actioncreate.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/function/function.h"
#include "omush/database/helpers.h"

namespace omush {

  CommandCreate::CommandCreate() : ICommand("@CREATE") {
  }

  CommandInfo CommandCreate::process(CommandContext& context) {
    CommandInfo info;
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);

    info.switches = splitStringIntoSegments(cmdSplit[0], "/", 10);
    info.switches.erase(info.switches.begin());

    if (cmdSplit.size() <= 1) {
      info.errorString = "Create what?";
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
      std::string arg1  = processExpression(eqSplit[1], context.funcScope).basicString();
      info.errorString = "@CREATE doesn't take more than one argument";
    }

    if (!info.switches.empty()) {
      info.errorString = "@CREATE doesn't take any switches.";
    }

    return info;
  }

  bool CommandCreate::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    ActionCreate create(context);
    create.name(info.eqArgs[0][0]);
    create.enact();
    return true;
  }

}  // namespace omush
