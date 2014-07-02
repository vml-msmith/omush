/**
 * \file actionsetflag.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONSETFLAG_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONSETFLAG_H_

#include <string>
#include "omush/database/definitions.h"
#include "omush/command/commandcontext.h"
#include "omush/action/action.h"

namespace omush {
  class Game;

  namespace database {
    class DatabaseObject;
  } // namespace Database

  class ActionSetFlag : public Action {
   public:
    ActionSetFlag(CommandContext& context);
    ActionSetFlag& target(database::DatabaseObject* object);
    ActionSetFlag& flag(Flag* flag);
    ActionSetFlag& unset();

    void enact();

   protected:
    database::DatabaseObject* target_;
    Flag* flag_;
    bool set_;
    CommandContext context_;

  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONSETFLAG_H_
