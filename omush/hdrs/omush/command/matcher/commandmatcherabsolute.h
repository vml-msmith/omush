/**
 * \file commandmatcherabsolute.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERABSOLUTE_H_
#define OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERABSOLUTE_H_

#include <string>
#include "omush/command/matcher/icommandmatcher.h"
#include "omush/command/command.h"

namespace omush {
  class CommandMatcherAbsolute : public ICommandMatcher {
   public:
    CommandMatcherAbsolute();
    ICommand* matchByString(CommandList, std::string str);
    ICommand* matchByString(std::string str);
    ICommand* contextMatchByString(CommandList list,
                                   CommandContext& context,
                                   std::string str);
    bool isCached();
   private:
    CommandMap cachedMap_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_MATCHER_COMMANDMATCHERABSOLUTE_H_
