/**
 * \file commandmatcherexit.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcherexit.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/command.h"
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/database/targetmatcher.h"

namespace omush {
  CommandMatcherExit::CommandMatcherExit()
    : ICommandMatcher(true) {
  }

  ICommand* CommandMatcherExit::matchByString(CommandList list,
                                                 std::string str) {
    // Clear out any static map.
    cachedMap_.clear();

    BOOST_FOREACH(ICommand* cmd, list) {
      std::string name = cmd->name();
      cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
    }

    return matchByString(str);
  }

  ICommand* CommandMatcherExit::matchByString(std::string str) {
    if (cachedMap_.empty())
      return NULL;

    boost::to_upper(str);

    if (cachedMap_.find(str) == cachedMap_.end())
      return NULL;

    return cachedMap_[str];
  }


  ICommand* CommandMatcherExit::contextMatchByString(CommandList list,
                                                     CommandContext& context,
                                                     std::string str) {
    database::DatabaseObject* executor = context.db->findObjectByDbref(context.executor);
    database::DatabaseObject* what = context.db->findObjectByDbref(executor->location());
    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher(context.db, executor);
    matcher.type(database::DbObjectTypeExit);
    matches = matcher.match(str);

    if (matches.size() > 0) {
      if (cachedMap_.empty()) {
        return this->matchByString(list, "GO");
      }
      return this->matchByString("GO");
    }

    return NULL;
  }

  bool CommandMatcherExit::isCached() {
    return false;
  }
}  // namespace omush
