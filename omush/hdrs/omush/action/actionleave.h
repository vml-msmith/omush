/**
 * \file actionleave.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include <string>
#include "omush/notifier.h"

namespace omush {
  class Game;

  class ActionLeave : public Action {
   public:
    ActionLeave(database::Database *db,
                Game *game,
                database::DatabaseObject *object);
    void enact();
    std::string constructString(database::DatabaseObject *listener,
                                StringDictionary dictionary);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_
