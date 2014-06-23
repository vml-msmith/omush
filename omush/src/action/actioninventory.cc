/**
 * \file actioninventory.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actioninventory.h"
//#include "omush/nameformatter.h"
//#include "omush/function/function.h"

namespace omush {
  ActionInventory::ActionInventory(CommandContext& context) : context_(context) {
  }

  void ActionInventory::enact() {
    std::string response = "Inventory..";
    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor, response);
  }
}  // namespace omush
