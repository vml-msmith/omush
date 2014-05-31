/**
 * \file actionarrive.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionarrive.h"
#include "omush/nameformatter.h"
#include "omush/notifier.h"
#include <boost/bind.hpp>

namespace omush {
  ActionArrive::ActionArrive(database::Database *db,
                           Game *game,
                           database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
  }

  std::string ActionArrive::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {
    if (listener == object_) {
      return "You have arrived.";
    }

    return NameFormatter(listener).formatInline(object_) + " arrives.";
  }

  void ActionArrive::enact() {
    Notifier notify(*game_, *db_);
    notify.notifySurroundings(object_,
                              boost::bind(&omush::ActionArrive::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
  }

}  // namespace omush
