/**
 * \file actionexamine.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONEXAMINE_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONEXAMINE_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"

namespace omush {
  class Game;

  class ActionExamine : public Action {
   public:
    ActionExamine(database::Database *db,
                  Game *game,
                  database::DatabaseObject *object);
    void enact();
    void enact(database::DatabaseObject* what);

   protected:
    database::DatabaseObject *what_;

    void notifyFatalError();
    void notifyEnactor(std::string msg);
    std::string nameLine();
    std::string descriptionLine();
    std::string typeLine();
    std::string ownerLine();
    std::string parentLine();
    std::string attributesLine();
    std::string exitsLine();
    std::string contentsLine();

    database::DatabaseObject* enactor;
    database::DatabaseObject* target ;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONEXAMINE_H_
