/**
 * \file commandmatcheruserdefined.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcheruserdefined.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/command.h"
#include "omush/utility.h"
#include <boost/regex.hpp>
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include "omush/database/helpers.h"

namespace omush {

  CommandMatcherUserDefined::CommandMatcherUserDefined()
    : ICommandMatcher(true) {
  }

  ICommand* CommandMatcherUserDefined::matchByString(CommandList list,
                                                     std::string str) {
    return NULL;
  }

  ICommand* CommandMatcherUserDefined::matchByString(std::string str) {
    return NULL;
  }

  ICommand* CommandMatcherUserDefined::contextMatchByString(CommandList list,
                                                                CommandContext& context,
                                                                std::string str) {
    using namespace database;
    DatabaseObject* executor = dbrefToObject(*(context.db), context.executor);
    DatabaseObject* location = dbrefToObject(*(context.db), executor->location());
    AttributeMap attributes = location->attributes();

    for (database::AttributeMap::iterator i = attributes.begin(); i != attributes.end(); ++i) {
      std::string value = i->second.value;
      if (value.length() == 0)
        continue;

      if (value.c_str()[0] == '$') {
        std::vector<std::string> parts = splitStringIntoSegments(value.substr(1,value.length()), ":", 2);
        std::string target = parts[0];

        boost::cmatch what;
        boost::regex expression(target);
        if (regex_match(str.c_str(), what, expression)) {
          std::cout << "matched" << std::endl;
        }
      }
    }

    // Room
    // Object in room (non player)
    // Object
    // inventory
    // Division
    // Zone object for location
    // Empireof the player
    // Universal zone object
    // Master room
    return NULL;
  }

  bool CommandMatcherUserDefined::isCached() {
    return false;
  }

}  // namespace omush
