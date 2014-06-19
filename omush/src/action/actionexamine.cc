/**
 * \file actionexamine.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionexamine.h"
#include "omush/nameformatter.h"

#include "omush/function/function.h"

namespace omush {
  ActionExamine::ActionExamine(database::Database *db,
                               Game *game,
                               database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    what_ = NULL;
  }

  void ActionExamine::enact() {
    enact(db_->findObjectByDbref(object_->location()));
  }

  void ActionExamine::enact(database::DatabaseObject* what) {
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

    // Name
    response += NameFormatter(what_).format(what_);

    // Description
    database::DatabaseAttribute descAttr = what_->getAttribute("description");
    std::string desc = descAttr.value;
    if (desc.length() > 0) {
      response += "\n";
      response += desc;
    }

    // Type
    std::string type = "UNKNOWN";

    switch(what_->type()) {
    case database::DbObjectTypePlayer:
      type = "Player";
      break;
    case database::DbObjectTypeExit:
      type = "Exit";
      break;
    case database::DbObjectTypeRoom:
      type = "Room";
      break;
    }
    boost::to_upper(type);
    response += "\nType: " + type;
    response += " Flags: ";

    // Owner
    database::DatabaseObject* owner = db_->findObjectByDbref(what_->owner());
    std::string ownerStr = "UNKNOWN";
    if (owner != NULL) {
      ownerStr = NameFormatter(object_).format(owner);
    }

    response += "\nOwner: " + ownerStr;


    // Parents:
    response += "\nParents: ";

    // Attributes...
    database::AttributeMap attributes = what_->attributes();
    for (database::AttributeMap::iterator i = attributes.begin(); i != attributes.end(); ++i) {
      response += "\n" + i->first + ":" + i->second.value;
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

    response += "\nHome: ";
    response += "\nLocation: ";


    Notifier(*game_, *db_).notify(object_, response);
  }
}  // namespace omush
