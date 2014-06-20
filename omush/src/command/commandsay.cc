/**
 * \file commandsay.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandsay.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action/actionsay.h"

namespace omush {


  CommandSay::CommandSay() : ICommand("SAY") {
  }

  bool CommandSay::run(std::string calledAs, std::string input, CommandContext context) {
    std::string words = "";
    std::vector<std::string> inputParts = splitStringIntoSegments(input, " ", 2);
    if (inputParts.size() > 1) {
      words = inputParts[1];
    }

    if (context.db->findObjectByDbref(context.executor) == NULL) {
      std::cout << "it's null " << context.enactor << std::endl;
    }

    ActionSay(context.db,
              context.game,
              context.db->findObjectByDbref(context.executor)).what(words).enact();
    return true;
  }

}  // namespace omush
