/**
 * \file actionpose.cc
 *
 * Copyright 2014 Michael Smith
 */

#include <boost/bind.hpp>
#import "omush/action/actionpose.h"
#import "omush/nameformatter.h"

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

    return NameFormatter(listener).formatInline(object_) + " " + what_;
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
