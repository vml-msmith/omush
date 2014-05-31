/**
 * \file actionconnect.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/action/actionconnect.h"
#import "omush/action/actionlook.h"

namespace omush {
  ActionConnect::ActionConnect(database::Database *db,
                               Game *game,
                               database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
  }

  void ActionConnect::enact() {
    ActionLook(db_, game_, object_).enact();
  }
}  // namespace omush
