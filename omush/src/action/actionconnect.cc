/**
 * \file actionconnect.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/action/actionconnect.h"
#import "omush/action/actionlook.h"

namespace omush {
  ActionConnect::ActionConnect(CommandContext& context) : context_(context) {
  }

  void ActionConnect::enact() {
    ActionLook(context_).enact();
  }
}  // namespace omush
