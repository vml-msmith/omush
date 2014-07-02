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

    std::vector<std::string> powerLevels;
    powerLevels.push_back("");
    powerLevels.push_back(", Lower Class");
    powerLevels.push_back(" In Empire");
    powerLevels.push_back(" In Empire, Lower Class");
    powerLevels.push_back(" In Division");
    powerLevels.push_back(" In Division, Lower Class");

    std::map<std::string,Power> allPowers = context_.db->powers.getAllPowers();
    for (std::map<std::string,Power>::iterator it = allPowers.begin();
         it != allPowers.end();
         ++it) {
      std::string msg = "";
      for (int level = 1; level <= 5; ++level) {
        if (database::hasPower(*context_.db, player_, it->first, level)) {
          msg = context_.db->powers.powerToName(&(it->second), level);
          break;
        }
      }
      if (msg.length() == 0) {
        if (database::hasPower(*context_.db, player_, it->first)) {
          msg = context_.db->powers.powerToName(&(it->second), 0);
        }
      }

      if (msg.length() > 0) {
          Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                            msg);
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
