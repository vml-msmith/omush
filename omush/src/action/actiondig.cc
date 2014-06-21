/**
 * \file actiondig.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actiondig.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/nameformatter.h"

namespace omush {
  ActionDig::ActionDig(CommandContext& context) : context_(context), name_(""), newRoom(NULL) {
  }

  ActionDig& ActionDig::name(std::string value) {
    name_ = value;
  }

  void ActionDig::enact() {
    if (name_.length() == 0) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "That's a silly name for a room.");
      // Not gonna work.
    }
    database::DatabaseObject *r1 = database::DatabaseObjectFactory::createRoom(context_.db);
    r1->setProperty("name", name_);

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                  NameFormatter(object_).format(r1) + " has been dug.");

    newRoom = r1;
  }
}  // namespace omush
