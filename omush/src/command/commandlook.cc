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

  bool CommandLook::run(std::string calledAs, std::string input, CommandContext context) {
    //    Command::run_(calledAs, input, context);

    database::DatabaseObject* enactor = context.db->findObjectByDbref(context.dbref);
    database::DatabaseObject* what = context.db->findObjectByDbref(enactor->location());

    std::vector<std::string> inputParts = splitStringIntoSegments(input, " ", 2);
    if (inputParts.size() > 1) {
      std::string words = inputParts[1];
      std::vector<database::DatabaseObject*> matches;
      matches = database::TargetMatcher(context.db, enactor).match(words);
      if (matches.size() > 1) {
        Notifier(*(context.game), *(context.db)).notify(enactor,
                                                        "I don't know which one you mean.");
        return true;
      }
      if (matches.size() == 0) {
        Notifier(*(context.game), *(context.db)).notify(enactor,
                                                        "I don't see that here.");
        return true;
      }
      ActionLook(context.db, context.game, context.db->findObjectByDbref(context.dbref)).enact(matches[0]);
      return true;
    }
    ActionLook(context.db, context.game, context.db->findObjectByDbref(context.dbref)).enact();
    return true;

  }

}  // namespace omush
