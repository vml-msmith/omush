/**
 * \file actionsetattribute.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/action/actionsetattribute.h"

namespace omush {
  ActionSetAttribute::ActionSetAttribute(database::Database *db,
                                         Game *game,
                                         database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
    //    what_ = "";
  }

  ActionSetAttribute& ActionSetAttribute::object(database::DatabaseObject* object) {
    what_ = object;
  }
  ActionSetAttribute& ActionSetAttribute::attribute(std::string attribute) {
    attribute_ = attribute;
  }
  ActionSetAttribute& ActionSetAttribute::value(std::string value) {
    value_ = value;
  }


  void ActionSetAttribute::enact() {
    what_->setAttribute(attribute_, value_);
    std::string message = attribute_ + " set.";
    what_->printAttrs();
    Notifier(*game_, *db_).notify(object_, message);
    //    Notifier(*game_, *db_).notify(object_, what_);
  }
}  // namespace omush
