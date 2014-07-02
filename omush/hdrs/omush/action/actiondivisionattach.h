/**
 * \file actiondivisionattach.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONDIVISIONATTACH_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONDIVISIONATTACH_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }
  class Game;

  class ActionDivisionAttach : public Action {
   public:
    ActionDivisionAttach(CommandContext& context);
    void enact();
    ActionDivisionAttach& target(database::DatabaseObject* value);
    ActionDivisionAttach& division(database::DatabaseObject* division);

    bool hasPermissionTarget();
    bool hasPermissionDivision();

   protected:
    CommandContext& context_;
    database::DatabaseObject* target_;
    database::DatabaseObject* division_;

    bool hasPermission();
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONDIVISIONATTACH_H_
