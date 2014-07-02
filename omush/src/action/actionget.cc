/**
 * \file actionget.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionget.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"
#include "omush/action/actiongo.h"

namespace omush {
  ActionGet::ActionGet(CommandContext& context) : context_(context), target_(NULL) {
  }

  ActionGet& ActionGet::target(database::DatabaseObject* target) {
    target_ = target;
    return *this;
  }


  bool ActionGet::hasPermission() {
    return true;
  }

  std::string ActionGet::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {
    if (listener == context_.cmdScope.executor) {
      return "You get " + nameFormatter(*(context_.db),listener).formatInline(target_) + ".";
    }

    return nameFormatter(*(context_.db),listener).formatInline(context_.cmdScope.executor) + " gets \"" + nameFormatter(*(context_.db),listener).formatInline(target_) + "\".";
  }


  void ActionGet::enact() {
    if (!hasPermission()) {
            Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                                "You don't have permission to get that.");
      return;
    }

    database::DatabaseObject* loc = context_.cmdScope.executor;
    if (loc == NULL) {
      return;
    }

    CommandContext newContext = context_;
    newContext.cmdScope.executor = target_;

    Notifier notify(*(context_.game), *(context_.db));
    notify.notifySurroundings(context_.cmdScope.executor,
                              boost::bind(&omush::ActionGet::constructString,
                                          this,
                                          ::_1,
                                          ::_2));

    ActionGo(newContext).enact(loc);
  }
}  // namespace omush
