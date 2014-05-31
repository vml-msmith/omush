/**
 * \file actiongo.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"

namespace omush {
  class Game;

  class ActionGo : public Action {
   public:
    ActionGo(database::Database *db,
             Game *game,
             database::DatabaseObject *object);
    void enact();
    void enact(database::DatabaseObject* what);

   protected:
    database::DatabaseObject *what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_
