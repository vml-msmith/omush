/**
 * \file actiongo.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/action/actiongo.h"
#import "omush/nameformatter.h"
#import "omush/action/actionlook.h"
#import "omush/action/actionarrive.h"
#import "omush/action/actionleave.h"
#import "omush/command/commandcontext.h"

namespace omush {
  ActionGo::ActionGo(CommandContext& context)
    : context_(context) {
  }

  void ActionGo::enact() {
    //    enact(db_->findObjectByDbref(object_->location()));
  }

  void ActionGo::enact(database::DatabaseObject* what) {
    what_ = what;

    if (what_ == NULL) {
      // TODO(msmith): This needs some sort of message.
      return;
    }

    ActionLeave(context_).enact();

    if (what_->type() == database::DbObjectTypeExit) {
      std::cout << "HERE" << std::endl;
      what_ = context_.db->findObjectByDbref(what_->home());
    }

    context_.db->moveObject(context_.cmdScope.executor, what_);
    std::cout << "2After" << std::endl;
    ActionArrive(context_).enact();
    std::cout << "3After" << std::endl;
    ActionLook(context_).enact();
  }
}  // namespace omush
