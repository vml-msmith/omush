/**
 * \file actionsay.cc
 *
 * Copyright 2014 Michael Smith
 */

#include <boost/bind.hpp>
#import "omush/action/actionsay.h"
#import "omush/database/utilityfactories.h"

namespace omush {
  ActionSay::ActionSay(CommandContext& context) : context_(context) {
    what_ = "";
  }

  ActionSay& ActionSay::what(std::string str) {
    what_ = str;
    return *this;
  }

  std::string ActionSay::constructString(database::DatabaseObject *listener,
                                         StringDictionary dictionary) {
    if (listener == context_.cmdScope.executor) {
      return "You say, \"" + what_ + "\"";
    }

    return nameFormatter(*(context_.db),listener).formatInline(context_.cmdScope.executor) + " says \"" + what_ + "\"";
  }

  void ActionSay::enact() {
    Notifier notify(*(context_.game), *(context_.db));
    notify.notifySurroundings(context_.cmdScope.executor,
                              boost::bind(&omush::ActionSay::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
  }
}  // namespace omush
