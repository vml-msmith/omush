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
#include <boost/bind.hpp>
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/database/targetmatcher.h"
#include <boost/scoped_ptr.hpp>
#include "omush/command/commandgo.h"
#include "omush/utility.h"

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

  ICommand* CommandMatcherAbsolute::contextMatchByString(CommandList list,
                                                         CommandContext& context,
                                                         std::string str) {
    return NULL;
  }

  bool CommandMatcherAbsolute::isCached() {
    return !cachedMap_.empty();
  }

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

  CommandMatcherExit::CommandMatcherExit()
    : ICommandMatcher(true) {
  }


  ICommand* CommandMatcherExit::matchByString(CommandList list,
                                                 std::string str) {
    // Clear out any static map.
    cachedMap_.clear();

    BOOST_FOREACH(ICommand* cmd, list) {
      std::string name = cmd->name();
      cachedMap_.insert(std::pair<std::string, ICommand*>(name, cmd));
    }

    return matchByString(str);
  }

  ICommand* CommandMatcherExit::matchByString(std::string str) {
    if (cachedMap_.empty())
      return NULL;

    boost::to_upper(str);

    if (cachedMap_.find(str) == cachedMap_.end())
      return NULL;

    return cachedMap_[str];
  }


  ICommand* CommandMatcherExit::contextMatchByString(CommandList list,
                                                     CommandContext& context,
                                                     std::string str) {
    database::DatabaseObject* enactor = context.db->findObjectByDbref(context.dbref);
    database::DatabaseObject* what = context.db->findObjectByDbref(enactor->location());
    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher(context.db, enactor);
    matcher.type(database::DbObjectTypeExit);
    matches = matcher.match(str);

    if (matches.size() > 0) {
      if (cachedMap_.empty()) {
        return this->matchByString(list, "GO");
      }
      return this->matchByString("GO");
    }

    return NULL;
  }

  bool CommandMatcherExit::isCached() {
    return false;
  }


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
