/**
 * \file actionthink.cc
 *
 * Copyright 2014 Michael Smith
 */


#import "omush/action/actionthink.h"
#import "omush/function/function.h"

namespace omush {
  ActionThink::ActionThink(database::Database *db,
                           Game *game,
                           database::DatabaseObject *object) {
    db_ = db;
    object_ = object;
    game_ = game;
    what_ = "";
  }

  ActionThink& ActionThink::what(std::string str) {
    what_ = str;
    return *this;
  }


  void ActionThink::enact() {
    std::string o = processExpression(what_).internalString();
    std::cout << "out " << o << std::endl;
    std::cout << ColorString::color("Test?", "red");
    Notifier(*game_, *db_).notify(object_,
                                  o);
  }
}  // namespace omush
