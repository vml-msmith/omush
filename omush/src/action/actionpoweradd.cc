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
    : context_(context), player_(NULL), level_(0) {
  }

  void ActionPowerAdd::enact() {
    if (player_ == NULL) {
      player_ = context_.cmdScope.executor;
    }


    if (power_ == NULL)
      return;


    database::addPowerByBit(player_, power_->bit, level_);
    // Test powers.
    std::string name = context_.db->powers.powerToName(power_, level_);

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Power '" + name + "' granted to " + NameFormatter(context_.cmdScope.executor).formatInline(player_) + ".");
    Notifier(*(context_.game), *(context_.db)).notify(player_,
                                                      "You have been granted the power '" + name + "' by " + NameFormatter(player_).formatInline(context_.cmdScope.executor) + ".");
  }

  ActionPowerAdd& ActionPowerAdd::player(database::DatabaseObject *player) {
    player_ = player;
    return *this;
  }

  ActionPowerAdd& ActionPowerAdd::power(Power* p) {
    power_ = p;
    return *this;
  }

  ActionPowerAdd& ActionPowerAdd::level(int level) {
    level_ = level;
    return *this;
  }

}  // namespace omush
