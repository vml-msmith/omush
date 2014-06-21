/**
 * \file commandmatcherattributesetter.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERATTRIBUTESETTER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERATTRIBUTESETTER_H_

#include <string>
#include "omush/command/matcher/icommandmatcher.h"
#include "omush/command/icommand.h"

namespace omush {
  class CommandMatcherAttributeSetter : public ICommandMatcher {
   public:
    CommandMatcherAttributeSetter(std::map<std::string,std::string> list);
    ICommand* matchByString(CommandList, std::string str);
    ICommand* matchByString(std::string str);
    ICommand* contextMatchByString(CommandList list,
                                   CommandContext& context,
                                   std::string str);
    bool isCached();
   private:
    CommandMap cachedMap_;
    std::map<std::string, std::string> cachedAttributeMap_;
    std::string firstWord(std::string input);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERPATTERN_H_
