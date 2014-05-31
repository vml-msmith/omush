/**
 * \file actionpose.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONPOSE_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONPOSE_H_

#include <string>
#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/notifier.h"

namespace omush {
  class Game;

  class ActionPose : public Action {
   public:
    ActionPose(database::Database *db,
               Game *game,
               database::DatabaseObject *object);
    ActionPose& what(std::string str);
    std::string constructString(database::DatabaseObject *listener,
                                StringDictionary dictionary);
    void enact();

   protected:
    std::string what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
