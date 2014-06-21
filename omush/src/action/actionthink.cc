/**
 * \file actionthink.cc
 *
 * Copyright 2014 Michael Smith
 */


#import "omush/action/actionthink.h"
#import "omush/function/function.h"

namespace omush {
  ActionThink::ActionThink(CommandContext& context) : context_(context) {

  }

  ActionThink& ActionThink::what(std::string str) {
    what_ = str;
    return *this;
  }


  void ActionThink::enact() {
    std::string o = processExpression(what_, context_.funcScope).internalString();

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                  o);
  }
}  // namespace omush
