/**
 * \file actiongo.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/action/actiongo.h"
#import "omush/nameformatter.h"
#import "omush/action/actionlook.h"
#import "omush/action/actionarrive.h"
#import "omush/action/actionleave.h"

namespace omush {
  ActionGo::ActionGo(database::Database *db,
                     Game *game,
                     database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    what_ = NULL;
  }

  void ActionGo::enact() {
    //    enact(db_->findObjectByDbref(object_->location()));
  }

  void ActionGo::enact(database::DatabaseObject* what) {
    what_ = what;

    if (what_ == NULL) {
      // TODO(msmith): This needs some sort of message.
      return;
    }

    ActionLeave(db_, game_, object_).enact();
    if (what_->type() == database::DbObjectTypeExit) {
      what_ = db_->findObjectByDbref(what_->home());
    }
    db_->moveObject(object_, what_);

    ActionArrive(db_, game_, object_).enact();
    ActionLook(db_, game_, object_).enact();
  }
}  // namespace omush
