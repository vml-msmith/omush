/**
 * \file commandmatcherpartial.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/matcher/commandmatcherpartial.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "omush/command/command.h"

#include <iostream>

namespace omush {

  CommandMatcherPartial::CommandMatcherPartial()
    : ICommandMatcher(false) {
    nodes_ = NULL;
  }

  CommandMatcherPartial::~CommandMatcherPartial() {
    clearPrefixTree();
  }

  void CommandMatcherPartial::clearPrefixNode(CommandPrefixNode* n) {
    for (std::map<char,CommandPrefixNode*>::iterator i = n->nodes.begin();
         i != n->nodes.end();
         ++i) {
      clearPrefixNode((i->second));
    }
    delete n;
    return;
  }

  void CommandMatcherPartial::clearPrefixTree() {
    if (nodes_ != NULL)
      clearPrefixNode(nodes_);
  }


  ICommand* CommandMatcherPartial::matchByString(CommandList list,
                                                 std::string str) {
    // Clear out any static map.
    clearPrefixTree();

    nodes_ = new CommandPrefixNode();
    CommandPrefixNode* currentNode = nodes_;
    BOOST_FOREACH(ICommand* cmd, list) {
      currentNode = nodes_;
      std::string cmdName = cmd->name();
      for (int i = 0; i < cmdName.size(); ++i) {
        currentNode->str = cmdName.substr(0,i);
        char c = cmdName.substr(i,1).c_str()[0];
        if (currentNode->nodes.find(c) == currentNode->nodes.end()) {
          currentNode->nodes.insert(std::make_pair(c, new CommandPrefixNode()));
        }
        currentNode = currentNode->nodes[c];
      }

      currentNode->cmd = cmd;
    }


    return matchByString(str);
  }

  CommandPrefixNode* CommandMatcherPartial::getCommandNode(CommandPrefixNode *node,
                                                           std::string str,
                                                           std::size_t pos) {
    char c = str.substr(pos,1).c_str()[0];

    if (node->nodes.find(c) == node->nodes.end()) {
      return NULL;
    }

    if (pos == str.length() - 1) {
      return node->nodes[c];
    }

    return getCommandNode(node->nodes[c], str, pos + 1);
  }


  ICommand* CommandMatcherPartial::matchByString(std::string str) {
    if (nodes_ == NULL)
      return NULL;


    std::vector<ICommand*> cmds;
    std::queue<CommandPrefixNode*> nodes;
    str = firstWord(str);
    boost::to_upper(str);
    nodes.push(getCommandNode(nodes_, str, 0));
    while (!nodes.empty()) {
      CommandPrefixNode* n = nodes.front();
      nodes.pop();
      if (n == NULL) {
        continue;
      }

      int i = 0;
      for (std::map<char,CommandPrefixNode*>::iterator it = n->nodes.begin();
           it != n->nodes.end();
           ++it) {
        nodes.push(it->second);
        ++i;
      }
      if (n->cmd != NULL) {
        cmds.push_back(n->cmd);
      }
    }

    if (cmds.size() == 1) {
      return cmds.front();
    }

    return NULL;
  }

  ICommand* CommandMatcherPartial::contextMatchByString(CommandList list,
                                                        CommandContext& context,
                                                        std::string str) {
    return NULL;
  }

  std::string CommandMatcherPartial::firstWord(std::string input) {
    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));
    if (inputParts.size() == 0) {
      return "";
    }

    std::string command = inputParts[0];
    return command;
  }

  bool CommandMatcherPartial::isCached() {
    return false;
  }
}  // namespace omush
