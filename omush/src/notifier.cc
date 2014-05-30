/**
 * \file notifier.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/notifier.h"
#import "omush/game.h"
#import <iostream>
#import "omush/database.h"
//#import "utility.h"

namespace omush {
  Notifier::Notifier(Game &game) : game_(game) {
  }

  void Notifier::notify(database::DatabaseObject *object, std::string str) {
    lowNotify(object->ref(), str);
  }
  void Notifier::lowNotify(database::dbref ref, std::string str) {
    std::cout << str << std::endl;
    ClientMap& clients = game_.clientList();
    for (ClientMap::iterator iter = clients.begin();
         iter != clients.end();
         ++iter) {

      std::cout << iter->second.ref << std::endl;

      if (iter->second.ref == ref) {
        game_.sendNetworkMessage(iter->first, str);
      }
    }
  }
  /*
  void Notifier::notifySurroundings(database::DatabaseObject object,
                                    std::string str) {
    //    Notifier::notifySurroundings

  }
  */

  Notifier& Notifier::exclude(database::dbref ref) {
    exclude_.push_back(ref);
    return *this;
  }
}  // namespace omush