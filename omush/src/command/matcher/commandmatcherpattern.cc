/**
 * \file commandmatcherpattern.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcherpattern.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/command.h"

namespace omush {
  CommandMatcherPattern::CommandMatcherPattern()
    : ICommandMatcher(false) {
  }

  ICommand* CommandMatcherPattern::matchByString(CommandList list,
                                                 std::string str) {
    // Clear out any static map.
    cachedMap_.clear();

    BOOST_FOREACH(ICommand* cmd, list) {
      std::string name = cmd->name();
      cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
    }

    return matchByString(str);
  }

  ICommand* CommandMatcherPattern::matchByString(std::string str) {
    if (cachedMap_.empty())
      return NULL;

    str = firstWord(str);
    boost::to_upper(str);

    if (cachedMap_.find(str) == cachedMap_.end())
      return NULL;

    return cachedMap_[str];
  }

  ICommand* CommandMatcherPattern::contextMatchByString(CommandList list,
                                                        CommandContext& context,
                                                        std::string str) {
    return NULL;
  }

  std::string CommandMatcherPattern::firstWord(std::string input) {
    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));
    if (inputParts.size() == 0) {
      return "";
    }

    std::string command = inputParts[0];
    return command;
  }

  bool CommandMatcherPattern::isCached() {
    return !cachedMap_.empty();
  }
}  // namespace omush
