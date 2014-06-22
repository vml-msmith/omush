/**
 * \file actiongo.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  class Game;

  class ActionGo : public Action {
   public:
    ActionGo(CommandContext& context);
    void enact();
    void enact(database::DatabaseObject* what);

   protected:
CommandContext& context_;
    database::DatabaseObject *what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONGO_H_
