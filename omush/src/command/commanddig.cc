/**
 * \file commanddig.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/command/commanddig.h"
#include <boost/algorithm/string.hpp>
#include "omush/action/actiondig.h"
#include "omush/action/actionopen.h"

/*
#include "omush/utility.h"
#include "omush/database/database.h"
#include "omush/action/actionsetattribute.h"
#include "omush/database/targetmatcher.h"
*/
namespace omush {
  CommandDig::CommandDig() : ICommand("@DIG") {
  }

  bool CommandDig::run(std::string calledAs,
                       std::string input,
                       CommandContext context) {
    database::DatabaseObject* executor = context.db->findObjectByDbref(context.executor);
    // @dig[/teleport] <roomname>[=<exitname>;<alias>*,<exitname>;<alias>*]

    // start with naive: @dig name
    std::vector<std::string> inputParts = splitStringIntoSegments(context.modifiedInput, " ", 2);

    if (inputParts.size() < 2) {
      Notifier(*(context.game), *(context.db)).notify(executor,
                                                      "What do you want to dig?");
      return true;
    }

    std::vector<std::string> eqParts = splitStringIntoSegments(inputParts[1], "=", 2);
    ActionDig dig(context.db, context.game, executor);
    dig.enact(eqParts[0]);
    if (eqParts.size() > 1) {
      std::cout << "HerE" << std::endl;
      // Try adding exits.
      if (dig.newRoom != NULL) {
        std::cout << "HerE again " << eqParts[1] << std::endl;
        std::vector<std::string> commaParts = splitStringIntoSegments(eqParts[1], ",", 2);
        if (commaParts.size() > 2) {
          std::cout << "more" << std::endl;
          Notifier(*(context.game), *(context.db)).notify(executor,
                                                          "Too many exits specified.");
        }
        else {
          ActionOpen(context.db, context.game, executor).from(context.db->findObjectByDbref(executor->location())).to(dig.newRoom).named(commaParts[0]).enact();
          if (commaParts.size() == 2) {
            ActionOpen(context.db, context.game, executor).from(dig.newRoom).to(context.db->findObjectByDbref(executor->location())).named(commaParts[1]).enact();
          }
        }
      }
    }
    return true;

    /*
    database::DatabaseObject* enactor = context.db->findObjectByDbref(context.dbref);

    std::string words = "";
    std::vector<std::string> inputParts = splitStringIntoSegments(context.modifiedInput, " ", 2);
    if (inputParts.size() < 2) {
      this->notify(context,enactor,"I can't see that here.");
      return true;
    }

    std::vector<std::string> eqParts = splitStringIntoSegments(inputParts[1], "=", 2);
    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher(context.db, enactor);

    matches = matcher.match(eqParts[0]);
    if (matches.size() == 0) {
      Notifier(*(context.game), *(context.db)).notify(enactor,
                                                      "I can't see that here.");
      return true;
    }

    if (matches.size() > 1) {
      Notifier(*(context.game), *(context.db)).notify(enactor,
                                                      "I don't know which one you mean.");
      return true;
    }

    if (eqParts.size() < 2) {
      Notifier(*(context.game), *(context.db)).notify(enactor,
                                                      "What do you want to set?");
    }

    // This is wher we need to decide if the first word is @set.
    std::vector<std::string> colonParts = splitStringIntoSegments(eqParts[1], ":", 2);
    if (colonParts.size() > 1) {
      // Attribute set
      ActionSetAttribute(context.db, context.game, context.db->findObjectByDbref(context.dbref))
        .object(matches[0])
        .attribute(colonParts[0])
        .value(colonParts[1])
        .enact();
      return true;
    }
    else {
      this->notify(context,enactor,"Flag set: Not implemented");
    }


//    ActionThink(context.db, context.game, context.db->findObjectByDbref(context.dbref)).what(words).enact();
    return true;
    */
    return true;
  }

}  // namespace omush
