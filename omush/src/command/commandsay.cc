/**
 * \file commandsay.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandsay.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action/actionsay.h"
#import "omush/function/function.h"
namespace omush {


  CommandSay::CommandSay() : ICommand("SAY") {
  }

  bool CommandSay::run(CommandContext& context) {
    std::string words = "";
    std::string cmd = context.cmdScope.currentString;
    std::vector<std::string> inputParts = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (inputParts.size() > 1) {
      words = inputParts[1];
    }

    words = processExpression(words, context.funcScope).internalString();

    ActionSay(context).what(words).enact();
    return true;
  }

}  // namespace omush
