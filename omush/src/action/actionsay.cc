/**
 * \file actionsay.cc
 *
 * Copyright 2014 Michael Smith
 */

#include <boost/bind.hpp>
#import "omush/action/actionsay.h"
#import "omush/nameformatter.h"

namespace omush {
  ActionSay::ActionSay(database::Database *db,
                       Game *game,
                       database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
    what_ = "";
  }

  ActionSay& ActionSay::what(std::string str) {
    what_ = str;
    return *this;
  }

  std::string ActionSay::constructString(database::DatabaseObject *listener,
                                         StringDictionary dictionary) {
    if (listener == object_) {
      return "You say, \"" + what_ + "\"";
    }

    return NameFormatter(listener).formatInline(object_) + " says \"" + what_ + "\"";
  }

  void ActionSay::enact() {
    Notifier notify(*game_, *db_);
//    StringDictionary dict;

    notify.notifySurroundings(object_,
                              boost::bind(&omush::ActionSay::constructString,
                                          this,
                                          ::_1,
                                          ::_2));
    //    notify.exclude(object_);
    //notify.notifySurroundings(object_, Nameformatter() " say, \"" + what_ + "\"")
      //    Notifier(*game_).exclude(object_).notify(object_->dbref(), "You say, \"" + what_ + "\"");
  }
}  // namespace omush
