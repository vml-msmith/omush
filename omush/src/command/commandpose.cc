/**
 * \file commandpose.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandpose.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action/actionpose.h"

namespace omush {
  CommandPose::CommandPose() : ICommand("POSE") {
    /*    shortCode_.push_back(":");*/
  }

  bool CommandPose::run(CommandContext& context) {
    std::string words = "";
    std::vector<std::string> inputParts = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (inputParts.size() > 1) {
      words = inputParts[1];
    }

    ActionPose(context.db, context.game, context.db->findObjectByDbref(context.executor)).what(words).enact();
    return true;
  }

}  // namespace omush
