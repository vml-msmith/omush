/**
 * \file actionsetattribute.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONSETATTRIBUTE_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONSETATTRIBUTE_H_

#include <string>
#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/notifier.h"

namespace omush {
  class Game;

  namespace database {
    class DatabaseObject;
  } // namespace Database

  class ActionSetAttribute : public Action {
   public:
    ActionSetAttribute(database::Database *db,
                       Game *game,
                       database::DatabaseObject *object);
    ActionSetAttribute& object(database::DatabaseObject* object);
    ActionSetAttribute& attribute(std::string attribute);
    ActionSetAttribute& value(std::string value);
    void enact();

   protected:
    database::DatabaseObject* what_;
    std::string attribute_;
    std::string  value_;

  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONSETATTRIBUTE_H_
