/**
 * \file actionpowerremove.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionpowerremove.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include "omush/power.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"

namespace omush {

  ActionPowerRemove::ActionPowerRemove(CommandContext& context)
    : context_(context), player_(NULL) {
  }

  void ActionPowerRemove::enact() {
    if (player_ == NULL) {
      player_ = context_.cmdScope.executor;
    }


    if (power_ == NULL)
      return;

    database::removePowerByBit(player_, power_->bit);
    // Test powers.
    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Power '" + power_->name + "' removed from " + nameFormatter(*(context_.db),context_.cmdScope.executor).formatInline(player_) + ".");
    Notifier(*(context_.game), *(context_.db)).notify(player_,
                                                      "You power '" + power_->name + "' has been removed by " + nameFormatter(*(context_.db), player_).formatInline(context_.cmdScope.executor) + ".");
  }

  ActionPowerRemove& ActionPowerRemove::player(database::DatabaseObject *player) {
    player_ = player;
    return *this;
  }

  ActionPowerRemove& ActionPowerRemove::power(Power* p) {
    power_ = p;
    return *this;
  }

}  // namespace omush
