/**
 * \file commandlook.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandlook.h"
//#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action/actionlook.h"
#include "omush/database/targetmatcher.h"

namespace omush {
  CommandLook::CommandLook() : ICommand("LOOK") {
  }

  bool CommandLook::run(CommandContext& context) {
    database::DatabaseObject* executor = context.db->findObjectByDbref(context.executor);
    database::DatabaseObject* what = context.db->findObjectByDbref(executor->location());

    std::vector<std::string> inputParts = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (inputParts.size() > 1) {
      std::string words = inputParts[1];
      std::vector<database::DatabaseObject*> matches;
      matches = database::TargetMatcher(context.db, executor).match(words);
      if (matches.size() > 1) {
        Notifier(*(context.game), *(context.db)).notify(executor,
                                                        "I don't know which one you mean.");
        return true;
      }
      if (matches.size() == 0) {
        Notifier(*(context.game), *(context.db)).notify(executor,
                                                        "I don't see that here.");
        return true;
      }
      ActionLook(context).enact(matches[0]);
      return true;
    }
    ActionLook(context).enact();
    return true;

  }

}  // namespace omush
