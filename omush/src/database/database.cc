/**
 * \file database.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include <map>

namespace omush {
  namespace database {
    DatabaseObject* Database::findObjectByDbref(Dbref ref) {
      if (allObjects_.find(ref) == allObjects_.end()) {
        return NULL;
      }

      return allObjects_[ref];
    }

    bool Database::hasObjectByRef(Dbref dbref) {
      return (allObjects_.find(dbref) != allObjects_.end());
    }


    bool Database::addObject(DatabaseObject* obj) {
      if (obj->ref() >= top_) {
        ++top_;
      }

      allObjects_.insert(std::make_pair(obj->ref(), obj));
      typedObjects_[obj->type_].insert(std::make_pair(obj->ref(), obj));
      // Move it to it's new home.

      DatabaseObject *location = findObjectByDbref(obj->location());
      if (location == NULL) {
        obj->location_ = -1;
      }

      moveObject(obj, location);
      return true;
    }

    void Database::moveObject(DatabaseObject* obj, DatabaseObject *location) {
      // Remove from the old location.
      DatabaseObject *oldLocation = findObjectByDbref(obj->location());
      oldLocation->removeFromContents(obj->ref());
      obj->location_ = location->ref();

      if (obj->type_ != DbObjectTypeRoom) {
        location->addToContents(obj->ref());
      }
    }

    Dbref Database::getNextRef() {
      return top_;
    }


  }  // namespace database
}  // namespace omush
