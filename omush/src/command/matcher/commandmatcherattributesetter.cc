/**
 * \file commandmatcherattributesetter.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcherattributesetter.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/command.h"
#include "omush/utility.h"

namespace omush {
  CommandMatcherAttributeSetter::CommandMatcherAttributeSetter(std::map<std::string, std::string> list)
    : ICommandMatcher(true) {
    cachedAttributeMap_ = list;
  }

  ICommand* CommandMatcherAttributeSetter::matchByString(CommandList list,
                                                         std::string str) {
    char firstChar = str.c_str()[0];
    if (firstChar != '@') {
      return NULL;
    }

    str = firstWord(str).substr(1, str.length());
    boost::to_upper(str);

    if (cachedAttributeMap_.find(str) == cachedAttributeMap_.end())
      return NULL;

    std::string newCommand = cachedAttributeMap_[str];
    BOOST_FOREACH(ICommand* cmd, list) {
      std::string name = cmd->name();
      cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
    }

    if (cachedMap_.find("@SET") == cachedMap_.end())
      return NULL;

    return cachedMap_["@SET"];
  }

  std::string CommandMatcherAttributeSetter::firstWord(std::string input) {
    // Lookup by command specifc.
    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));
    if (inputParts.size() == 0) {
      return "";
    }

    std::string command = inputParts[0];
    return command;
  }


  ICommand* CommandMatcherAttributeSetter::matchByString(std::string str) {
    return NULL;
  }


  ICommand* CommandMatcherAttributeSetter::contextMatchByString(CommandList list,
                                                                CommandContext& context,
                                                                std::string str) {
    char firstChar = str.c_str()[0];
    if (firstChar != '@') {
      return NULL;
    }

    std::string first = firstWord(str).substr(1, str.length());
    boost::to_upper(first);

    if (cachedAttributeMap_.find(first) == cachedAttributeMap_.end())
      return NULL;

    std::string newCommand = cachedAttributeMap_[first];
    // TODO: Move this out.
    if (cachedMap_.empty()) {
      BOOST_FOREACH(ICommand* cmd, list) {
        std::string name = cmd->name();
        cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
      }
    }

    if (cachedMap_.find("@SET") == cachedMap_.end())
      return NULL;


    std::vector<std::string> inputParts = splitStringIntoSegments(str, " ", 2);
    if (inputParts.size() < 2) {
      context.modifiedInput = "@SET";
    }
    else {
      std::vector<std::string> eqParts = splitStringIntoSegments(inputParts[1], "=", 2);
      if (eqParts.size() == 2) {
        context.modifiedInput = "@SET " + eqParts[0] + "=" + newCommand + ":" + eqParts[1];
      }
    }

    return cachedMap_["@SET"];
  }

  bool CommandMatcherAttributeSetter::isCached() {
    return false;
  }
}  // namespace omush
