/**
 * \file actionconnect.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_ACTIONCONNECT_H_
#define OMUSH_HDRS_OMUSH_ACTION_ACTIONCONNECT_H_

#include "omush/action/action.h"
#include "omush/command/commandcontext.h"

namespace omush {

  class ActionConnect : public Action {
   public:
    ActionConnect(CommandContext& context);
    void enact();
   private:
    CommandContext& context_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ACTION_ACTIONLOOK_H_
