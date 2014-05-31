/**
 * \file commandparser.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandparser.h"
#include "omush/command/command.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

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

    BOOST_FOREACH(std::string s, cmd->shortCodes()) {
      commandShortCodeDictionary_.insert(std::make_pair(s, cmd));
    }
  }

  bool CommandParser::run(std::string input, CommandContext context) {
    Command *cmd = lookupByName(input);

    // Lookup absolute.
    if (cmd != NULL) {
      cmd->run(input, input, context);
      return true;
    }

    // Lookup by short code.
    std::string shortCode = input.substr(0,1);
    cmd = lookupByShortCode(shortCode);
    if (cmd != NULL) {
      cmd->run(cmd->name(),
               cmd->name() + " " + input.substr(1,input.length()),
               context);
      return true;
    }

    // Finally lookup by command.
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
      return true;
    }


    return false;
  }


  Command* CommandParser::lookupByShortCode(std::string shortCode) {
    if (commandShortCodeDictionary_.find(shortCode) == commandShortCodeDictionary_.end()) {
      return NULL;
    }
    return (commandShortCodeDictionary_.find(shortCode)->second);
  }

  Command* CommandParser::lookupByName(std::string name) {
    if (commandDictionary_.find(name) == commandDictionary_.end()) {
      return NULL;
    }
    return (commandDictionary_.find(name)->second);
  }

}  // namespace omush
