/**
 * \file actioncreatedivision.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actioncreatedivision.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  ActionCreateDivision::ActionCreateDivision(CommandContext& context)
    : context_(context), name_(""), newRoom(NULL) {
  }

  ActionCreateDivision& ActionCreateDivision::name(std::string value) {
    name_ = value;
    return *this;
  }


  bool ActionCreateDivision::hasPermission() {
    return database::hasPower(*(context_.db),
                              context_.cmdScope.executor,
                              "Create Division");
  }

  bool ActionCreateDivision::hasQuota() {
    return database::hasQuota(*(context_.db),
                              context_.cmdScope.executor,
                              quotaCost());
  }

  bool ActionCreateDivision::hasCredit() {
    return database::hasCredit(*(context_.db),
                               context_.cmdScope.executor,
                               creditCost());
  }

  int ActionCreateDivision::quotaCost() {
    return 1;
  }

  int ActionCreateDivision::creditCost() {
    return 10000;
  }

  void ActionCreateDivision::enact() {
    if (name_.length() == 0) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "That's a silly name for a division.");
      // Not gonna work.
    }

    if (!hasPermission()) {
            Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                              "You don't have permission to create a division.");
      return;
    }

    if (!hasQuota()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available quota to create a division.");
      return;
    }

    if (!hasCredit()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available credit to create a division.");
      return;
    }


    database::DatabaseObject *r1 = database::DatabaseObjectFactory::createDivision(context_.db);
    r1->setProperty("name", name_);

    context_.db->changeOwnership(r1, context_.cmdScope.executor);
    context_.db->moveObject(r1, context_.cmdScope.executor);

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Created " + nameFormatter(*(context_.db),object_).format(r1) + ".");

    newRoom = r1;
  }
}  // namespace omush
