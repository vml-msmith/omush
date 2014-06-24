/**
 * \file actionpowerlist.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionpowerlist.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include "omush/power.h"
#include "omush/database/helpers.h"

namespace omush {

  ActionPowerList::ActionPowerList(CommandContext& context)
    : context_(context), player_(NULL) {
  }

  void ActionPowerList::enact() {
    if (player_ == NULL) {
      player_ = context_.cmdScope.executor;
    }

    // Test powers.


    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Power List..\n----------");

    std::map<std::string,Power> allPowers = context_.db->powers.getAllPowers();
    for (std::map<std::string,Power>::iterator it = allPowers.begin();
         it != allPowers.end();
         ++it) {
      if (database::hasPower(*context_.db, player_, it->first)) {
        Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          it->second.name);
      }
    }

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "----------");
  }

  ActionPowerList& ActionPowerList::player(database::DatabaseObject *player) {
    player_ = player;
    return *this;
  }

}  // namespace omush
