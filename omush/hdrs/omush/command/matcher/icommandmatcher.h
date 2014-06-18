/**
 * \file icommandmatcher.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_MATCHER_ICOMMANDMATCHER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_MATCHER_ICOMMANDMATCHER_H_

#include <string>
#include "omush/command/command.h"

namespace omush {
  class ICommandMatcher {
   public:
    explicit ICommandMatcher(bool contextAware)
      : hasContextMatch_(contextAware) {}
    virtual ICommand* matchByString(CommandList list, std::string str) = 0;
    virtual ICommand* matchByString(std::string str) = 0;
    virtual ICommand* contextMatchByString(CommandList list,
                                           CommandContext& context,
                                           std::string str) = 0;
    virtual bool isCached() = 0;
    bool hasContextMatch() { return hasContextMatch_; }
   private:
    bool hasContextMatch_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_MATCHER_ICOMMANDMATCHER_H_
