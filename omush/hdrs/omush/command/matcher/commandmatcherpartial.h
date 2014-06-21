/**
 * \file commandmatcherpartial.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERPARTIAL_H_
#define OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERPARTIAL_H_

#include <string>
#include "omush/command/matcher/icommandmatcher.h"
#include "omush/command/icommand.h"

namespace omush {
  struct CommandPrefixNode;
  struct CommandPrefixNode {
    ICommand* cmd;
    std::string str;
    std::map<char,CommandPrefixNode*> nodes;
    CommandPrefixNode() {
      cmd = NULL;
    }
  };

  class CommandMatcherPartial : public ICommandMatcher {
   public:
    CommandMatcherPartial();
    ~CommandMatcherPartial();
    ICommand* matchByString(CommandList, std::string str);
    ICommand* matchByString(std::string str);
    ICommand* contextMatchByString(CommandList list,
                                   CommandContext& context,
                                   std::string str);
    bool isCached();
   protected:
    void clearPrefixNode(CommandPrefixNode* n);
    void clearPrefixTree();
    CommandPrefixNode* getCommandNode(CommandPrefixNode *node,
                                      std::string str,
                                      std::size_t pos);

   private:
    CommandPrefixNode* nodes_;
    std::string firstWord(std::string input);
    CommandMap cachedMap_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERPATTERN_H_
