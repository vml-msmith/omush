/**
 * \file commandparser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_

#include "omush/command/icommand.h"
#include "omush/command/matcher/icommandmatcher.h"
#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace omush {
  class CommandParser {
  public:
    void registerMatcher(ICommandMatcher* matcher);
    void registerCommand(ICommand* command);
    ICommand* findCommand(std::string str, CommandContext& context);

  private:
    std::vector<ICommandMatcher*> commandMatchers_;
    CommandList commandList_;
  };
}

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
