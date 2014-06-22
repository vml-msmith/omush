/**
 * \file actionlook.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionlook.h"
#include "omush/nameformatter.h"
#include "omush/function/function.h"

namespace omush {
  ActionLook::ActionLook(CommandContext& context) : context_(context) {
  }

  void ActionLook::enact() {
    enact((context_.db)->findObjectByDbref(context_.cmdScope.executor->location()));
  }

  void ActionLook::enact(database::DatabaseObject* what) {
    what_ = what;

    if (what_ == NULL) {
      what_ = db_->findObjectByDbref(context_.cmdScope.executor->location());
    }
    if (what_ == NULL) {
      std::cout << "Nill" << std::endl;
      // Notify can't see the object.
      return;
    }
    std::string response = "";
    response += NameFormatter(context_.cmdScope.executor).format(what_);

    database::DatabaseAttribute descAttr = what_->getAttribute("description");

    std::string desc = descAttr.value;
    if (desc.length() > 0) {
      response += "\n";

      FunctionScope* s = new FunctionScope();
      s->enactor = context_.cmdScope.executor;
      s->caller = what_;
      s->executor = what_;

      response += processExpression(desc, s).outString();
      delete s;
    }

    std::vector<database::Dbref> contents = what_->contents();
    std::string contentString = "";
    std::string exitString = "";
    for (std::vector<database::Dbref>::iterator iter = contents.begin();
         iter != contents.end();
         ++iter) {
      database::DatabaseObject *c = context_.db->findObjectByDbref(*iter);

      if (c->type() == database::DbObjectTypeExit) {
        exitString += "\n" + NameFormatter(context_.cmdScope.executor).noDbref().format(c);
        std::cout << exitString << std::endl;
      }
      else if (*iter != context_.cmdScope.executor->dbref()) {
        contentString += "\n";
        contentString += NameFormatter(context_.cmdScope.executor).format(c);
      }
    }
    if (exitString.length() > 0) {
      response += "\nExits:" + exitString;
    }
    if (contentString.length() > 0) {
      response += "\nContents:" + contentString;
    }

    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor, response);
  }
}  // namespace omush
