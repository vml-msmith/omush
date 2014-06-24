/**
 * \file actionpoweradd.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionpoweradd.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include "omush/power.h"
#include "omush/database/helpers.h"
#include "omush/nameformatter.h"

namespace omush {

  ActionPowerAdd::ActionPowerAdd(CommandContext& context)
    : context_(context), player_(NULL) {
  }

  void ActionPowerAdd::enact() {
    if (player_ == NULL) {
      player_ = context_.cmdScope.executor;
    }


    if (power_ == NULL)
      return;


    database::addPowerByBit(player_, power_->bit);
    // Test powers.
    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Power '" + power_->name + "' granted to " + NameFormatter(context_.cmdScope.executor).formatInline(player_) + ".");
    Notifier(*(context_.game), *(context_.db)).notify(player_,
                                                      "You have been granted the power '" + power_->name + "' by " + NameFormatter(player_).formatInline(context_.cmdScope.executor) + ".");
  }

  ActionPowerAdd& ActionPowerAdd::player(database::DatabaseObject *player) {
    player_ = player;
    return *this;
  }

  ActionPowerAdd& ActionPowerAdd::power(Power* p) {
    power_ = p;
    return *this;
  }

}  // namespace omush
