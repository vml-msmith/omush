/**
 * \file commandmatcherabsulute.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcherabsolute.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/icommand.h"

namespace omush {
  CommandMatcherAbsolute::CommandMatcherAbsolute()
    : ICommandMatcher(false) {
  }

  ICommand* CommandMatcherAbsolute::matchByString(CommandList list,
                                                  std::string str) {
    // Clear out any static map.
    cachedMap_.clear();

    BOOST_FOREACH(ICommand* cmd, list) {
      std::string name = cmd->name();
      cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
    }

    return matchByString(str);
  }

  ICommand* CommandMatcherAbsolute::matchByString(std::string str) {
    if (cachedMap_.empty())
      return NULL;

    boost::to_upper(str);
    if (cachedMap_.find(str) == cachedMap_.end())
      return NULL;

    return cachedMap_[str];
  }

  ICommand*
  CommandMatcherAbsolute::contextMatchByString(CommandList list,
                                               CommandContext& context,
                                               std::string str) {
    return NULL;
  }

  bool CommandMatcherAbsolute::isCached() {
    return !cachedMap_.empty();
  }
}  // namespace omush
