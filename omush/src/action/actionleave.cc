/**
 * \file actionleave.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionleave.h"
#include "omush/nameformatter.h"
#include "omush/notifier.h"
#include <boost/bind.hpp>

namespace omush {
  ActionLeave::ActionLeave(database::Database *db,
                           Game *game,
                           database::DatabaseObject *object) {
    game_ = game;
    db_ = db;
    object_ = object;
  }

  std::string ActionLeave::constructString(database::DatabaseObject *listener,
                                          StringDictionary dictionary) {
    if (listener == object_) {
      return "You have left.";
    }
std::cout << "Tell someone" << std::endl;
    return NameFormatter(listener).formatInline(object_) + " leaves.";
  }

  void ActionLeave::enact() {
std::cout << "Leave" << std::endl;
    Notifier notify(*game_, *db_);
    notify.notifySurroundings(object_,
                              boost::bind(&omush::ActionLeave::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
  }
}  // namespace omush
