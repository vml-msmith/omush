/**
 * \file commanddig.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commanddig.h"
#include <boost/algorithm/string.hpp>
#include "omush/action/actiondig.h"
#include "omush/action/actionopen.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/function/function.h"
#include "omush/database/helpers.h"

/*
#include "omush/utility.h"
#include "omush/database/database.h"
#include "omush/action/actionsetattribute.h"
#include "omush/database/targetmatcher.h"
*/
namespace omush {

  CommandDig::CommandDig() : ICommand("@DIG") {
  }

  CommandInfo CommandDig::process(CommandContext& context) {
    CommandInfo info;
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

    return info;
  }

  bool CommandDig::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      notifyExecutor(context, info.errorString);
      return true;
    }

    ActionDig dig(context);
    dig.name(info.eqArgs[0][0]);
    dig.enact();

    if (dig.newRoom != NULL) {
      database::DatabaseObject* location;
      location = objectLocation(*(context.db), context.cmdScope.executor);
      if (info.eqArgs.size() > 1 && info.eqArgs[1].size() > 0) {
        ActionOpen(context).from(location).to(dig.newRoom).name(info.eqArgs[1][0]).enact();
      }
      if (info.eqArgs.size() > 1 && info.eqArgs[1].size() > 1) {
        ActionOpen(context).from(dig.newRoom).to(location).name(info.eqArgs[1][1]).enact();
      }
    }
    return true;


  }

}  // namespace omush
