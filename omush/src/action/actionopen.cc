/**
 * \file actionopen.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionopen.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/nameformatter.h"

namespace omush {
  ActionOpen::ActionOpen(database::Database *db,
                         Game *game,
                         database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    name_ = "";
    from_ = NULL;
    to_ = NULL;

  }

  ActionOpen& ActionOpen::from(database::DatabaseObject* object) {
    from_ = object;
    return *this;
  }

  ActionOpen& ActionOpen::to(database::DatabaseObject* object) {
    to_ = object;
    return *this;
  }

  ActionOpen& ActionOpen::named(std::string value) {
    name_ = value;
    return *this;
  }

  void ActionOpen::enact() {
    if (from_ == NULL) {
      Notifier(*game_, *db_).notify(object_, "I don't know where you want to open the exit from. This should really never happen, so maybe you should tell someone.");
      return;
    }

    std::string name = name_;
    database::DatabaseObject *e1=database::DatabaseObjectFactory::createExit(db_);
    e1->setProperty("name", name);
    db_->moveObject(e1, from_);;
    Notifier(*game_, *db_).notify(object_, "Opened exit " + NameFormatter(object_).format(e1) + ".");

    if (to_ != NULL) {
      e1->home(to_->dbref());
      Notifier(*game_, *db_).notify(object_, "Linked exit " + NameFormatter(object_).format(e1) + " to " + NameFormatter(object_).format(to_) + "...");
    }
    /*
    database::DatabaseObject *e1=database::DatabaseObjectFactory::createExit(db_);
    e1->setProperty("name", str);
    Notifier(*game_, *db_).notify(object_, NameFormatter(object_).format(r1) + " has been opened.");
    */
  }
}  // namespace omush
