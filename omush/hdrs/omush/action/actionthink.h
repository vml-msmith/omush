/**
 * \file actionthink.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONTHINK_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONTHINK_H_

#include <string>
#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/notifier.h"

namespace omush {
  class Game;

  class ActionThink : public Action {
   public:
    ActionThink(database::Database *db,
                Game *game,
                database::DatabaseObject *object);
    ActionThink& what(std::string str);
    void enact();

   protected:
    std::string what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONTHINK_H_
