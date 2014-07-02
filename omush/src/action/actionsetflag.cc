/**
 * \file actionsetflag.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/action/actionsetflag.h"
#include "omush/flag.h"
#include "omush/database/helpers.h"

namespace omush {
  ActionSetFlag::ActionSetFlag(CommandContext& context)
    : context_(context) {
    flag_ = NULL;
    set_ = true;
    target_ = NULL;
  }

  ActionSetFlag& ActionSetFlag::object(database::DatabaseObject* object) {
    target_ = object;
    return *this;
  }

  ActionSetFlag& ActionSetFlag::flag(Flag* flag) {
    flag_ = flag;;
    return *this;
  }

  ActionSetFlag& ActionSetFlag::unset() {
    set_ = false;
    return *this;
  }


  void ActionSetFlag::enact() {
    if (target_ == NULL) {
      target_ = context_.cmdScope.executor;
    }

    if (flag_ == NULL)
      return;

    if (!hasPowerOver(*(context_.db),
                      context_.cmdScope.executor,
                      target_,
                      flag_->name)) {
      Notifier(*(context_.game),
               *(context_.db)).notify(context_.cmdScope.executor,
                                      "You don't have permission.");
      return;
    }

    if (set_) {
      addFlagByBit(target_, flag_->bit);
      Notifier(*(context_.game),
               *(context_.db)).notify(context_.cmdScope.executor,
                                      flag_->name + " set.");
    }
    else {
      removeFlagByBit(target_, flag_->bit);
      Notifier(*(context_.game),
               *(context_.db)).notify(context_.cmdScope.executor,
                                      flag_->name + " reset.");
    }
  }
}  // namespace omush
