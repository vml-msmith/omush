/**
 * \file commandparser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_

#include "omush/command/command.h"
#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace omush {
  class CommandParser {
  public:
    void registerCommand(Command *cmd);
    bool run(std::string input, CommandContext context);
    Command* lookupByName(std::string name);
    Command* lookupByShortCode(std::string shortCode);
    ~CommandParser();

  protected:
    std::map<std::string,Command*> commandDictionary_;
    std::map<std::string, Command*> commandShortCodeDictionary_;
  };
}

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
