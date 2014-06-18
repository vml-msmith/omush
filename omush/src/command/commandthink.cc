/**
 * \file commandthink.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commandthink.h"
#include <boost/algorithm/string.hpp>
#include "omush/utility.h"
#include "omush/database/database.h"
#include "omush/action/actionthink.h"

namespace omush {
  CommandThink::CommandThink() : ICommand("THINK") {
  }

  bool CommandThink::run(std::string calledAs, std::string input, CommandContext context) {
    std::string words = "";
    std::vector<std::string> inputParts = splitStringIntoSegments(input, " ", 2);
    if (inputParts.size() > 1) {
      words = inputParts[1];
    }

    ActionThink(context.db, context.game, context.db->findObjectByDbref(context.dbref)).what(words).enact();
    return true;
  }

}  // namespace omush
