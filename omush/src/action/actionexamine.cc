/**
 * \file actionexamine.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionexamine.h"
#include "omush/nameformatter.h"

#include "omush/function/function.h"
#include "omush/database/helpers.h"

namespace omush {
  ActionExamine::ActionExamine(database::Database *db,
                               Game *game,
                               database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
    what_ = NULL;

    enactor = object_;
    target = what_;
  }

  void ActionExamine::notifyEnactor(std::string msg) {
    Notifier(*game_, *db_).notify(enactor, msg);
  }

  std::string ActionExamine::nameLine() {
    if (target == NULL || enactor == NULL)
      return "";

    return NameFormatter(enactor).format(target);
  }

  std::string ActionExamine::descriptionLine() {
    if (target == NULL || enactor == NULL || db_ == NULL)
      return "";

    database::DatabaseAttribute descAttr = target->getAttribute("description");
    std::string desc = descAttr.value;
    if (desc.length() > 0) {
      return "\n" + desc;
    }

    return "";
  }

  std::string ActionExamine::typeLine() {
    if (target == NULL)
      return "";

    std::string type = objectTypeString(target);
    boost::to_upper(type);
    return "\nType: " + type;
  }

  std::string ActionExamine::ownerLine() {
    if (target == NULL || enactor == NULL || db_ == NULL)
      return "";

    return "\nOwner: " + NameFormatter(enactor).format(objectOwner(*(db_), target));
  }

  std::string ActionExamine::attributesLine() {
    if (target == NULL || enactor == NULL || db_ == NULL)
      return "";

    // TODO(msmith): Build the list of items into a vector and pass
    //               to another method to format. That way we can
    //               change the formatting eaiser.
    std::string response = "";
    database::AttributeMap attributes = target->attributes();
    for (database::AttributeMap::iterator i = attributes.begin();
         i != attributes.end();
         ++i) {
      response += "\n" + i->first + ":" + i->second.value;
    }

    return response;
  }

  std::string ActionExamine::contentsLine() {
    if (target == NULL || enactor == NULL || db_ == NULL)
      return "";

    // TODO(msmith): Build the list of items into a vector and pass
    //               to another method to format. That way we can
    //               optionally pass to @conformat.
    std::vector<database::Dbref> contents = target->contents();
    std::string contentString = "";

    for (std::vector<database::Dbref>::iterator iter = contents.begin();
         iter != contents.end();
         ++iter) {
      database::DatabaseObject *c = db_->findObjectByDbref(*iter);

      if (c->type() == database::DbObjectTypeExit)
        continue;

      if (*iter != object_->dbref()) {
        contentString += "\n";
        contentString += NameFormatter(enactor).format(c);
      }
    }

    if (contentString.length() == 0)
      return "";

    return "\nContents:\n" + contentString;
  }

  std::string ActionExamine::exitsLine() {
    if (target == NULL || enactor == NULL || db_ == NULL)
      return "";

    // TODO(msmith): Build the list of items into a vector and pass
    //               to another method to format. That way we can
    //               optionally pass to @exitformat.
    std::vector<database::Dbref> contents = target->contents();
    std::string contentString = "";

    for (std::vector<database::Dbref>::iterator iter = contents.begin();
         iter != contents.end();
         ++iter) {
      database::DatabaseObject *c = dbrefToObject(*(db_), *iter);

      if (c->type() == database::DbObjectTypeExit && *iter != object_->dbref()) {
        contentString += "\n" + NameFormatter(enactor).noDbref().format(c);
      }
    }

    if (contentString.length() == 0)
      return "";

    return "\nExits:\n" + contentString;
  }

  void ActionExamine::notifyFatalError() {
    if (enactor != NULL) {
      notifyEnactor("FATAL ERROR: Something went terribly wrong. This should probably be reported.");
    }

    // TODO(msmith): Log fatal error.
  }

  void ActionExamine::enact() {
    enact(db_->findObjectByDbref(enactor->location()));
  }

  void ActionExamine::enact(database::DatabaseObject* what) {
    if (db_ == NULL || enactor == NULL)
      return notifyFatalError();

    // TODO (msmith): Move this.
    target = what;

    using namespace database;
    FunctionExecutor fe;
    Database& db = *(db_);

    if (target == NULL) {
      target = objectLocation(db, enactor);
    }

    if (target == NULL) {
      notifyEnactor("I don't see that here.");
      return;
    }

    std::string response = "";

    // TODO(msmith): Put these in a vector and join at the end.
    //               That way maybe we can shuffle things around.
    response += nameLine();
    response += descriptionLine();
    response += typeLine();
    response += " Flags: ";
    response += ownerLine();
    response += "\nParents: ";
    response += attributesLine();
    response += exitsLine();
    response += contentsLine();
    response += "\nHome: ";
    response += "\nLocation: ";


    notifyEnactor(response);
  }
}  // namespace omush
