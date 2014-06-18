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
  ActionDig::ActionDig(database::Database *db,
                       Game *game,
                       database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    what_ = NULL;
    newRoom = NULL;
  }
  void ActionDig::enact() {
  }

  void ActionDig::enact(std::string str) {
    Notifier(*game_, *db_).notify(object_, "Digging");


    database::DatabaseObject *r1=database::DatabaseObjectFactory::createRoom(db_);
    r1->setProperty("name", str);
    Notifier(*game_, *db_).notify(object_, NameFormatter(object_).format(r1) + " has been dug.");
    newRoom = r1;
  }
}  // namespace omush
