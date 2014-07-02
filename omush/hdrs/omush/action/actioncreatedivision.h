/**
 * \file actioncreatedivision.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONCREATEDIVISION_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONCREATEDIVISION_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }
  class Game;

  class ActionCreateDivision : public Action {
   public:
    ActionCreateDivision(CommandContext& context);
    void enact();
    ActionCreateDivision& name(std::string value);

    database::DatabaseObject* newRoom;
   protected:
    CommandContext& context_;
    std::string name_;

    bool hasPermission();
    bool hasQuota();
    bool hasCredit();

    int quotaCost();
    int creditCost();
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONCREATEDIVISION_H_
