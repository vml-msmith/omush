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
  Notifier::Notifier(Game &game, database::Database& database) :
    game_(game), database_(database) {
  }

  void Notifier::notify(database::DatabaseObject *object, std::string str) {
    lowNotify(object, str);
  }

  void Notifier::lowNotify(database::DatabaseObject *object, std::string str) {
    database::Dbref dbref = object->dbref();

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

  void Notifier::notifySurroundings(database::DatabaseObject *object,
                                    notifyCallback callback) {

    std::map<database::Dbref, database::DatabaseObject*> listeners;

    // Add the object.
    listeners[object->dbref()] = object;


    database::DatabaseObject *location = database_.findObjectByDbref(object->location());
    if (location != NULL)  {
      // Add the location.
      listeners[location->dbref()] = location;

      // Add the location's contents.
      std::vector<database::Dbref> contents = location->contents();
      for (std::vector<database::Dbref>::iterator iter = contents.begin();
           iter != contents.end();
           ++iter) {
        database::DatabaseObject *obj = database_.findObjectByDbref(*iter);
        if (obj != NULL) {
          listeners[obj->dbref()] = obj;
        }
      }
    }

    // Add the contents of the item.
    std::vector<database::Dbref> inventory = object->contents();
    for (std::vector<database::Dbref>::iterator iter = inventory.begin();
         iter != inventory.end();
         ++iter) {
        database::DatabaseObject *obj = database_.findObjectByDbref(*iter);
        if (obj != NULL) {
          listeners[obj->dbref()] = obj;
        }
    }

    StringDictionary dict;
    for (database::DbMap::iterator iter = listeners.begin();
         iter != listeners.end();
         ++iter) {
      Notifier::lowNotify(iter->second, callback(iter->second, dict));
    }
  }

  /*
  void Notifier::notifySurroundings(database::DatabaseObject object,
                                    std::string str) {
    //    Notifier::notifySurroundings

  }
  */
  /*
  Notifier& Notifier::exclude(database::DatabaseObject *objcet) {
    exclude_.push_back(object);
    return *this;
    }*/

}  // namespace omush
