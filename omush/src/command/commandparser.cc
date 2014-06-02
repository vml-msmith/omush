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

  CommandParser::CommandParser() {
    nodes_ = new CommandNode();
  }

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

    std::string cmdName = cmd->name();
    CommandNode* currentNode = nodes_;
    for (int i = 0; i < cmdName.size(); ++i) {
      currentNode->str = cmdName.substr(0,i);
      char c = cmdName.substr(i,1).c_str()[0];
      if (currentNode->nodes.find(c) == currentNode->nodes.end()) {
        currentNode->nodes.insert(std::make_pair(c, new CommandNode()));
      }

      currentNode = currentNode->nodes[c];
    }
    currentNode->cmd = cmd;
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

    // Lookup by command specifc.
    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));
    std::string command = inputParts[0];
    std::vector<std::string> commandParts;
    boost::split(commandParts, command, boost::is_any_of("/"));
    command = commandParts[0];
    boost::to_upper(command);
    cmd = lookupByName(command);

    // Lookup by alias.


    // Lookup exits.


    // Lookup soft code.


    // Match by prefix.
    if (cmd == NULL) {
      std::vector<Command*> matches =  matchCommandByPrefix(command);
      if (matches.size() == 1) {
        cmd = matches.front();
      }
    }

    if (cmd != NULL) {
      cmd->run(command, input, context);
      return true;
    }

    return false;
  }


  CommandNode* CommandParser::getCommandNode(CommandNode *node, std::string str, std::size_t pos) {
    char c = str.substr(pos,1).c_str()[0];

    if (node->nodes.find(c) == node->nodes.end())
      return NULL;

    if (pos == str.length() - 1) {
      return node->nodes[c];
    }

    return getCommandNode(node->nodes[c], str, pos + 1);
  }

  std::vector<Command*> CommandParser::matchCommandByPrefix(std::string command) {
    std::vector<Command*> cmds;


    std::queue<CommandNode*> nodes;
    nodes.push(getCommandNode(nodes_, command, 0));
    while (!nodes.empty()) {
      CommandNode* n = nodes.front();
      nodes.pop();
      if (n == NULL) {
        continue;
      }

      int i = 0;
      for (std::map<char,CommandNode*>::iterator it = n->nodes.begin();
           it != n->nodes.end();
           ++it) {
        nodes.push(it->second);
        ++i;
      }
      if (n->cmd != NULL) {
        cmds.push_back(n->cmd);
      }
    }
    return cmds;
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
