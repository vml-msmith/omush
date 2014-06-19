/**
 * \file actionlook.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionlook.h"
#include "omush/nameformatter.h"

#include "omush/function/function.h"

namespace omush {
  ActionLook::ActionLook(database::Database *db,
                         Game *game,
                         database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    what_ = NULL;
  }

  void ActionLook::enact() {
    enact(db_->findObjectByDbref(object_->location()));
  }

  void ActionLook::enact(database::DatabaseObject* what) {
    FunctionExecutor fe;
    what_ = what;

    if (what_ == NULL) {
      what_ = db_->findObjectByDbref(object_->location());
    }
    if (what_ == NULL) {
      std::cout << "Nill" << std::endl;
      // Notify can't see the object.
      return;
    }
    std::string response = "";
    response += NameFormatter(object_).format(what_);

    database::DatabaseAttribute descAttr = what_->getAttribute("description");

    std::string desc = descAttr.value;
    if (desc.length() > 0) {
      response += "\n";
      response += fe.strParse(desc);
    }

    std::vector<database::Dbref> contents = what_->contents();
    std::string contentString = "";
    std::string exitString = "";
    for (std::vector<database::Dbref>::iterator iter = contents.begin();
         iter != contents.end();
         ++iter) {
      database::DatabaseObject *c = db_->findObjectByDbref(*iter);

      if (c->type() == database::DbObjectTypeExit) {
        exitString += "\n" + NameFormatter(object_).noDbref().format(c);
        std::cout << exitString << std::endl;
      }
      else if (*iter != object_->dbref()) {
        contentString += "\n";
        contentString += NameFormatter(object_).format(c);
      }
    }
    if (exitString.length() > 0) {
      response += "\nExits:" + exitString;
    }
    if (contentString.length() > 0) {
      response += "\nContents:" + contentString;
    }

    Notifier(*game_, *db_).notify(object_, response);
  }
}  // namespace omush
