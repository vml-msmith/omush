/**
 * \file notifier.cc
 *
 * Copyright 2014 Michael Smith
 */

#import "omush/notifier.h"
#import "omush/game.h"
#import <iostream>
#import "omush/database/database.h"
#import "omush/database/databaseobject.h"
//#import "utility.h"

namespace omush {
  Notifier::Notifier(Game &game) : game_(game) {
  }

  void Notifier::notify(database::DatabaseObject *object, std::string str) {
    lowNotify(object->dbref(), str);
  }
  void Notifier::lowNotify(database::Dbref dbref, std::string str) {
    std::cout << str << std::endl;
    ClientMap& clients = game_.clientList();
    for (ClientMap::iterator iter = clients.begin();
         iter != clients.end();
         ++iter) {

      std::cout << iter->second.dbref << std::endl;

      if (iter->second.dbref == dbref) {
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

  Notifier& Notifier::exclude(database::Dbref dbref) {
    exclude_.push_back(dbref);
    return *this;
  }
}  // namespace omush
