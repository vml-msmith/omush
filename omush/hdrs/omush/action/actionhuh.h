/**
 * \file actionhuh.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONHUH_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONHUH_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"

namespace omush {
  class Game;

  class ActionHuh : public Action {
  public:
    ActionHuh(database::Database *db,
              Game *game,
              database::DatabaseObject *object);
    void enact();
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONHUH_H_
