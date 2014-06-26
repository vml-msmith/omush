/**
 * \file actioninventory.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actioninventory.h"
#include <boost/foreach.hpp>
#include "omush/nameformatter.h"
#include "omush/database/helpers.h"

//#include "omush/function/function.h"

namespace omush {
  ActionInventory::ActionInventory(CommandContext& context) : context_(context) {
  }

  void ActionInventory::enact() {
    std::vector<database::Dbref> contents = context_.cmdScope.executor->contents();
    std::string message = "";

    if (contents.empty()) {
      message = "You aren't carrying anything.";
    }
    else {
      message = "You are carrying:";
      BOOST_FOREACH(database::Dbref content, contents) {
        database::DatabaseObject *item = dbrefToObject(*(context_.db), content);
        if (item == NULL) {
          continue;
        }

        message += "\n" + NameFormatter(context_.cmdScope.executor).format(item);
      }
    }

    message += "\nYou have ";
    if (hasPower(*(context_.db), context_.cmdScope.executor,
                 "unlimited credits")) {
      message += "unlimited credits.";
    }
    else {
      int credits = context_.cmdScope.executor->getCredit();
      message += boost::lexical_cast<std::string>(credits);
      if (credits == 1)
        message += "credit.";
      else
        message += " credits.";
    }

    Notifier(*(context_.game),
             *(context_.db)).notify(context_.cmdScope.executor,
                                    message);
  }
}  // namespace omush
