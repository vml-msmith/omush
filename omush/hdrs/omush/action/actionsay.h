/**
 * \file actionsay.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONSAY_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONSAY_H_

#include <string>
#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include "omush/notifier.h"
#include "omush/command/commandcontext.h"

namespace omush {
  class Game;

  class ActionSay : public Action {
   public:
    ActionSay(CommandContext& context);
    ActionSay& what(std::string str);
    std::string constructString(database::DatabaseObject *listener,
                                StringDictionary dictionary);
    void enact();

   protected:
    CommandContext& context_;
    std::string what_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
