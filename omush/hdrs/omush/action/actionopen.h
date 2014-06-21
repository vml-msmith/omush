/**
 * \file actionopen.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONOPEN_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONOPEN_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }
  class Game;

  class ActionOpen : public Action {
   public:
    ActionOpen(CommandContext& context);

    ActionOpen& from(database::DatabaseObject* object);
    ActionOpen& to(database::DatabaseObject* object);
    ActionOpen& name(std::string value);

    void enact();

    database::DatabaseObject* newExit;
   protected:
    database::DatabaseObject *from_;
    database::DatabaseObject *to_;
    CommandContext& context_;
    std::string name_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONOPEN_H_
