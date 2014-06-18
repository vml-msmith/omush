/**
 * \file actiondig.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONDIG_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONDIG_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }
  class Game;

  class ActionDig : public Action {
   public:
    ActionDig(database::Database *db,
              Game *game,
              database::DatabaseObject *object);
    void enact();
    void enact(std::string);

    database::DatabaseObject* newRoom;
   protected:
    database::DatabaseObject *what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONDIG_H_
