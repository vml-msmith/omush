/**
 * \file actionleave.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_

#include "omush/database/definitions.h"
#include "omush/action/action.h"
#include <string>
#include "omush/notifier.h"
#include "omush/command/commandcontext.h"

namespace omush {
  class Game;

  class ActionLeave : public Action {
   public:
    ActionLeave(CommandContext& context);
    void enact();
    std::string constructString(database::DatabaseObject *listener,
                                StringDictionary dictionary);
   private:
    CommandContext& context_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLEAVE_H_
