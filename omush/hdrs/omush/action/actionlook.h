/**
 * \file actionlook.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"

namespace omush {
  class Game;

  class ActionLook : public Action {
   public:
    ActionLook(database::Database *db,
               Game *game,
               database::DatabaseObject *object);
    void enact();

   protected:
    database::DatabaseObject *what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
