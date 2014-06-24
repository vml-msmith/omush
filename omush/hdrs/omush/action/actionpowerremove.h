/**
 * \file actionpowerremove.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTIONPOWERREMOVE_H_
#define OMUSH_HDRS_OMUSH_ACTIONPOWERREMOVE_H_

#include "omush/action/action.h"
#include "omush/command/commandcontext.h"
#include "omush/power.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }  // namespace database


  class ActionPowerRemove : public Action {
   public:
    ActionPowerRemove(CommandContext& context);
    void enact();
    ActionPowerRemove& player(database::DatabaseObject* player);
    ActionPowerRemove& power(Power *p);
   private:
    database::DatabaseObject* player_;
    CommandContext& context_;
    Power* power_;
  };

} // omush

#endif //  OMUSH_HDRS_OMUSH_ACTIONPOWERREMOVE_H_
