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
  struct CommandNode;
  struct CommandNode {
    Command* cmd;
    std::string str;
    std::map<char,CommandNode*> nodes;
    CommandNode() {
      cmd = NULL;
    }
  };

  class CommandParser {
  public:
    CommandParser();
    void registerCommand(Command *cmd);
    bool run(std::string input, CommandContext context);
    Command* lookupByName(std::string name);
    Command* lookupByShortCode(std::string shortCode);

    ~CommandParser();

  protected:
    std::vector<Command*> matchCommandByPrefix(std::string);
    CommandNode* getCommandNode(CommandNode *node, std::string str, std::size_t pos);

    CommandNode* nodes_;
    std::map<std::string,Command*> commandDictionary_;
    std::map<std::string, Command*> commandShortCodeDictionary_;
  };
}

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
