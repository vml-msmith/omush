/**
 * \file actionleave.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionleave.h"
#include "omush/database/utilityfactories.h"
#include "omush/notifier.h"
#include <boost/bind.hpp>
#include "omush/command/commandcontext.h"

namespace omush {
  ActionLeave::ActionLeave(CommandContext& context) : context_(context) {
  }

  std::string ActionLeave::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {
    if (listener == object_) {
      return "You have left.";
    }

    return nameFormatter(*(context_.db), listener).formatInline(object_) + " leaves.";
  }

  void ActionLeave::enact() {
/*
std::cout << "Leave" << std::endl;
Notifier notify(*(context_.game), *(context_.db));
    notify.notifySurroundings(object_,
                              boost::bind(&omush::ActionLeave::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
*/
  }
}  // namespace omush
