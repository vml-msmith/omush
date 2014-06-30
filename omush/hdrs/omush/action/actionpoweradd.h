/**
 * \file actionpoweradd.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTIONPOWERADD_H_
#define OMUSH_HDRS_OMUSH_ACTIONPOWERADD_H_

#include "omush/action/action.h"
#include "omush/command/commandcontext.h"
#include "omush/power.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }  // namespace database


  class ActionPowerAdd : public Action {
   public:
    ActionPowerAdd(CommandContext& context);
    void enact();
    ActionPowerAdd& player(database::DatabaseObject* player);
    ActionPowerAdd& power(Power *p);
    ActionPowerAdd& level(int level);
   private:
    database::DatabaseObject* player_;
    CommandContext& context_;
    Power* power_;
    int level_;
  };

} // omush

#endif //  OMUSH_HDRS_OMUSH_ACTIONPOWERREMOVE_H_
