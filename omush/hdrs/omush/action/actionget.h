/**
 * \file actionget.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONGET_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONGET_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }
  class Game;

  class ActionGet : public Action {
   public:
    ActionGet(CommandContext& context);
    void enact();
    ActionGet& target(database::DatabaseObject* target);
    std::string constructString(database::DatabaseObject *listener,
                                StringDictionary dictionary);
   protected:
    CommandContext& context_;
    database::DatabaseObject* target_;

    bool hasPermission();
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONGET_H_
