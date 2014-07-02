/**
 * \file actiondrop.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actiondrop.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"
#include "omush/action/actiongo.h"

namespace omush {
  ActionDrop::ActionDrop(CommandContext& context) : context_(context), target_(NULL) {
  }

  ActionDrop& ActionDrop::target(database::DatabaseObject* target) {
    target_ = target;
    return *this;
  }


  bool ActionDrop::hasPermission() {
    return true;
  }

  std::string ActionDrop::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {
    if (listener == context_.cmdScope.executor) {
      return "You drop " + nameFormatter(*(context_.db),listener).formatInline(target_) + ".";
    }

    return nameFormatter(*(context_.db),listener).formatInline(context_.cmdScope.executor) + " drops \"" + nameFormatter(*(context_.db),listener).formatInline(target_) + "\".";
  }


  void ActionDrop::enact() {
    if (!hasPermission()) {
            Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                                "You don't have permission to drop that here.");
      return;
    }

    database::DatabaseObject* loc = objectLocation(*(context_.db), context_.cmdScope.executor);
    if (loc == NULL) {
      return;
    }

    CommandContext newContext = context_;
    newContext.cmdScope.executor = target_;

    Notifier notify(*(context_.game), *(context_.db));
    notify.notifySurroundings(context_.cmdScope.executor,
                              boost::bind(&omush::ActionDrop::constructString,
                                          this,
                                          ::_1,
                                          ::_2));

    ActionGo(newContext).enact(loc);
  }
}  // namespace omush
