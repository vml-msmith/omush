/**
 * \file commandgo.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandgo.h"
#include "omush/database/database.h"
#include "omush/database/definitions.h"
#include "omush/action/actiongo.h"
#include "omush/database/targetmatcher.h"

namespace omush {
  CommandGo::CommandGo() : ICommand("GO") {
  }

  CommandGo* CommandGo::factory() {
    return new CommandGo();
  }

  bool CommandGo::run(CommandContext& context) {
    database::DatabaseObject* executor = context.db->findObjectByDbref(context.executor);
    database::DatabaseObject* what = context.db->findObjectByDbref(executor->location());

    std::vector<std::string> inputParts = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (inputParts.size() > 0) {
      if (!boost::iequals(inputParts[0], this->name())) {
        if (inputParts.size() > 1) {
          inputParts[1] = context.cmdScope.currentString;
        }
        inputParts.push_back(context.cmdScope.currentString);
      }
    }

    if (inputParts.size() > 1) {
      std::string words = inputParts[1];
      std::vector<database::DatabaseObject*> matches;
      database::TargetMatcher matcher(context.db, executor);
      matcher.type(database::DbObjectTypeExit);
      matches = matcher.match(words);
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
      ActionGo(context).enact(matches[0]);
      return true;
    }
    ActionGo(context).enact();
    return true;

  }

}  // namespace omush
