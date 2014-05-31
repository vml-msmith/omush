/**
 * \file actionhuh.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/action/actionhuh.h"

namespace omush {
  ActionHuh::ActionHuh(database::Database *db,
                       Game *game,
                       database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
  }

  void ActionHuh::enact() {
    Notifier(*game_, *db_).notify(object_, "Huh? I don't know that command.");
  }
}  // namespace omush
