/**
 * \file commandset.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commandset.h"
#include <boost/algorithm/string.hpp>

#include "omush/utility.h"
#include "omush/database/database.h"
#include "omush/action/actionsetattribute.h"
#include "omush/database/targetmatcher.h"

namespace omush {
  CommandSet::CommandSet() : ICommand("@SET") {
  }


  void CommandSet::notify(CommandContext context,
                          database::DatabaseObject *enactor,
                          std::string msg) {
    Notifier(*(context.game), *(context.db)).notify(enactor,
                                                    msg);
  }

  bool CommandSet::run(CommandContext& context) {
    database::DatabaseObject* executor = context.db->findObjectByDbref(context.executor);

    std::vector<std::string> inputParts = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (inputParts.size() < 2) {
      this->notify(context,executor,"I can't see that here.");
      return true;
    }

    std::vector<std::string> eqParts = splitStringIntoSegments(inputParts[1], "=", 2);
    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher(context.db, executor);

    matches = matcher.match(eqParts[0]);

    if (matches.size() == 0) {
      Notifier(*(context.game), *(context.db)).notify(executor,
                                                      "I can't see that here.");
      return true;
    }

    if (matches.size() > 1) {
      Notifier(*(context.game), *(context.db)).notify(executor,
                                                      "I don't know which one you mean.");
      return true;
    }

    if (eqParts.size() < 2) {
      Notifier(*(context.game), *(context.db)).notify(executor,
                                                      "What do you want to set?");
    }

    // This is wher we need to decide if the first word is @set.
    std::vector<std::string> colonParts = splitStringIntoSegments(eqParts[1], ":", 2);

    if (colonParts.size() > 1) {
      // Attribute set
      ActionSetAttribute(context.db, context.game, executor)
        .object(matches[0])
        .attribute(colonParts[0])
        .value(colonParts[1])
        .enact();
      return true;
    }
    else {
      this->notify(context,executor,"Flag set: Not implemented");
    }

//    ActionThink(context.db, context.game, context.db->findObjectByDbref(context.dbref)).what(words).enact();
    return true;
  }

}  // namespace omush
