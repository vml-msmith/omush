/**
 * \file actionpowerlist.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTIONPOWERLIST_H_
#define OMUSH_HDRS_OMUSH_ACTIONPOWERLIST_H_

#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  namespace database {
    class DatabaseObject;
  }  // namespace database

  class ActionPowerList : public Action {
   public:
    ActionPowerList(CommandContext& context);
    void enact();
    ActionPowerList& player(database::DatabaseObject* player);
   private:
    database::DatabaseObject* player_;
    CommandContext& context_;
  };

} // omush

#endif //  OMUSH_HDRS_OMUSH_ACTIONPOWERLIST_H_
