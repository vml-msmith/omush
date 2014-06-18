/**
 * \file commandparser.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/parser/commandparser.h"
#include "omush/command/command.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/database/targetmatcher.h"
#include <boost/scoped_ptr.hpp>
#include "omush/command/commandgo.h"
#include "omush/utility.h"

namespace omush {
  void CommandParser::registerMatcher(ICommandMatcher* matcher) {
    commandMatchers_.push_back(matcher);
  }

  void CommandParser::registerCommand(ICommand* command) {
    commandList_.push_back(command);
  }

  ICommand* CommandParser::findCommand(std::string cmd,
                                       CommandContext& context) {
    context.modifiedInput = cmd;
    BOOST_FOREACH(ICommandMatcher* matcher, commandMatchers_) {
      ICommand* c = NULL;
      if (matcher->hasContextMatch()) {
        c = matcher->contextMatchByString(commandList_, context, cmd);
      }
      else {
        // Need to cache commandList_ somehow.
        if (matcher->isCached())
          c = matcher->matchByString(cmd);
        else
          c = matcher->matchByString(commandList_, cmd);
      }

      if (c != NULL) {
        return c;
      }
    }

    return NULL;
  }


  /*
  void CommandRegistrar::registerCommandMatchCallback(CommandMatchCallback callback) {
    matchCallbacks_.push_back(callback);
  }

  void CommandRegistrar::registerCommand(Command *cmd) {
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


  void CommandParser::registerCommandMatchCallback(class callback, class object) {

  //  void CommandParser::registerCommandMatchCallback(CommandMatchCallback callBack) {
    matchCallbacks_boost::bind(callback,
                               object,
                               ::_1,
                               ::_2)

  //    matchCallbacks_.push_back(callBack);
  }

  CommandParser::CommandParser() {
    nodes_ = new CommandNode();
    setupCommands();
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
    for (std::vector<CommandMatchCallback>::iterator iter = matchCallbacks_.begin();
         iter != matchCallbacks_.end();
         ++iter) {
      if ((*iter)(input, context)) {
        return true;
        break;
      }
    }
    /*

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
    */

  /*
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
  */
}  // namespace omush
