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

  bool CommandGo::run(std::string calledAs, std::string input, CommandContext context) {
    database::DatabaseObject* enactor = context.db->findObjectByDbref(context.dbref);
    database::DatabaseObject* what = context.db->findObjectByDbref(enactor->location());

    std::vector<std::string> inputParts = splitStringIntoSegments(input, " ", 2);
    if (inputParts.size() > 0) {
      if (!boost::iequals(inputParts[0], this->name())) {
        if (inputParts.size() > 1) {
          inputParts[1] = input;
        }
        inputParts.push_back(input);
      }
    }
    if (inputParts.size() > 1) {
      std::string words = inputParts[1];
      std::vector<database::DatabaseObject*> matches;
      database::TargetMatcher matcher(context.db, enactor);
      matcher.type(database::DbObjectTypeExit);
      matches = matcher.match(words);
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
      ActionGo(context.db, context.game, context.db->findObjectByDbref(context.dbref)).enact(matches[0]);
      return true;
    }
    ActionGo(context.db, context.game, context.db->findObjectByDbref(context.dbref)).enact();
    return true;

  }

}  // namespace omush
