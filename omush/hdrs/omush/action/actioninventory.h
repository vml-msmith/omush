/**
 * \file actioninventory.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONINVENTORY_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONINVENTORY_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {
  class Game;

  class ActionInventory : public Action {
   public:
    ActionInventory(CommandContext& context);
    void enact();

   protected:
    CommandContext& context_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONINVENTORY_H_
