/**
 * \file actionconnect.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONCONNECT_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONCONNECT_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
namespace omush {
  class Game;

  class ActionConnect : public Action {
   public:
    ActionConnect(database::Database *db,
                  Game *game,
                  database::DatabaseObject *object);
    void enact();
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
