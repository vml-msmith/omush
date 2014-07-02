/**
 * \file actionpose.cc
 *
 * Copyright 2014 Michael Smith
 */

#include <boost/bind.hpp>
#include "omush/action/actionpose.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  ActionPose::ActionPose(database::Database *db,
                         Game *game,
                         database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
    what_ = "";
  }

  ActionPose& ActionPose::what(std::string str) {
    what_ = str;
    return *this;
  }

  std::string ActionPose::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {

    return nameFormatter(*(db_), listener).formatInline(object_) + " " + what_;
  }

  void ActionPose::enact() {
    Notifier notify(*game_, *db_);
    notify.notifySurroundings(object_,
                              boost::bind(&omush::ActionPose::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
  }
}  // namespace omush
