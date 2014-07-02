/**
 * \file actiondig.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actiondig.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  ActionDig::ActionDig(CommandContext& context) : context_(context), name_(""), newRoom(NULL) {
  }

  ActionDig& ActionDig::name(std::string value) {
    name_ = value;
    return *this;
  }


  bool ActionDig::hasPermission() {
    return true;
  }

  bool ActionDig::hasQuota() {
    return database::hasQuota(*(context_.db),
                              context_.cmdScope.executor,
                              quotaCost());
  }

  bool ActionDig::hasCredit() {
    return database::hasCredit(*(context_.db),
                               context_.cmdScope.executor,
                               creditCost());
  }

  int ActionDig::quotaCost() {
    return 1;
  }

  int ActionDig::creditCost() {
    return 10;
  }

  void ActionDig::enact() {
    if (name_.length() == 0) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "That's a silly name for a room.");
      // Not gonna work.
    }

    if (!hasPermission()) {
            Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                                "You don't have permission to dig a room.");
      return;
    }

    if (!hasQuota()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available quota to dig a room.");
      return;
    }

    if (!hasCredit()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available credit to dig a roomm.");
      return;
    }


    database::DatabaseObject *r1 = database::DatabaseObjectFactory::createRoom(context_.db);
    r1->setProperty("name", name_);

    context_.db->changeOwnership(r1, context_.cmdScope.executor);
    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      nameFormatter(*(context_.db), object_).format(r1) + " has been dug.");

    newRoom = r1;
  }
}  // namespace omush
