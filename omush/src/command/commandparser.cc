/**
 * \file commandparser.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandparser.h"
#include "omush/command/command.h"
#include <vector>
#include <boost/algorithm/string.hpp>

namespace omush {
  CommandParser::~CommandParser() {
    for (std::map<std::string,Command*>::iterator iter = commandDictionary_.begin();
         iter != commandDictionary_.end();
         ++iter) {
      delete iter->second;
    }
  }

  void CommandParser::registerCommand(Command *cmd) {
    commandDictionary_.insert(std::make_pair(cmd->name(), cmd));
  }

  bool CommandParser::run(std::string input, CommandContext context) {
    Command *cmd = lookupByName(input);

    if (cmd != NULL) {
      cmd->run(input, input, context);
      std::cout << "Run" << std::endl;
      return true;
    }

    // Lookup by short code.

    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));
    std::string command = inputParts[0];
    std::vector<std::string> commandParts;
    boost::split(commandParts, command, boost::is_any_of("/"));
    command = commandParts[0];
    boost::to_upper(command);
    cmd = lookupByName(command);

    if (cmd != NULL) {
      cmd->run(command, input, context);
      std::cout << "Run2" << std::endl;
      return true;
    }

    // Lookup by alias.


    return false;
  }

  Command* CommandParser::lookupByName(std::string name) {
    if (commandDictionary_.find(name) == commandDictionary_.end()) {
      return NULL;
    }
    return (commandDictionary_.find(name)->second);
  }

}  // namespace omush
