/**
 * \file actioncreate.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actioncreate.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  ActionCreate::ActionCreate(CommandContext& context)
    : context_(context), name_(""), newObject(NULL) {
  }

  ActionCreate& ActionCreate::name(std::string value) {
    name_ = value;
    return *this;
  }


  bool ActionCreate::hasPermission() {
    return true;
  }

  bool ActionCreate::hasQuota() {
    return database::hasQuota(*(context_.db),
                              context_.cmdScope.executor,
                              quotaCost());
  }

  bool ActionCreate::hasCredit() {
    return database::hasCredit(*(context_.db),
                               context_.cmdScope.executor,
                               creditCost());
  }

  int ActionCreate::quotaCost() {
    return 1;
  }

  int ActionCreate::creditCost() {
    return 10;
  }

  void ActionCreate::enact() {
    if (name_.length() == 0) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "That's a silly name for a thing.");
      // Not gonna work.
    }

    if (!hasPermission()) {
            Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                              "You don't have permission to create an object.");
      return;
    }

    if (!hasQuota()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available quota to create an object.");
      return;
    }

    if (!hasCredit()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "You don't have the available credit to create an object.");
      return;
    }


    database::DatabaseObject *r1 = database::DatabaseObjectFactory::createThing(context_.db);
    r1->setProperty("name", name_);

    context_.db->changeOwnership(r1, context_.cmdScope.executor);
    context_.db->moveObject(r1, context_.cmdScope.executor);

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "Created " + nameFormatter(*(context_.db), object_).format(r1) + ".");

    newObject = r1;
  }
}  // namespace omush
