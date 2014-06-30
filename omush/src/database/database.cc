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
    DatabaseObject* Database::findObjectByDbref(Dbref dbref) {
      if (allObjects_.find(dbref) == allObjects_.end()) {
        return NULL;
      }

      return allObjects_[dbref];
    }

    bool Database::hasObjectByDbref(Dbref dbref) {
      return (allObjects_.find(dbref) != allObjects_.end());
    }


    bool Database::addObject(DatabaseObject* obj) {
      if (obj->dbref() >= top_) {
        ++top_;
      }

      allObjects_.insert(std::make_pair(obj->dbref(), obj));
      typedObjects_[obj->type_].insert(std::make_pair(obj->dbref(), obj));
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
      oldLocation->removeFromContents(obj->dbref());
      obj->location_ = location->dbref();

      if (obj->type_ != DbObjectTypeRoom) {
        location->addToContents(obj->dbref());
      }
    }

    Dbref Database::getNextDbref() {
      return top_;
    }

    void Database::changeOwnership(DatabaseObject* obj, DatabaseObject *owner) {
      DatabaseObject *oldOwner = findObjectByDbref(obj->owner());

      if (oldOwner != NULL) {
        oldOwner->addQuota(-1);
      }

      owner->addQuota(1);

      obj->setOwner(owner);
    }

    void Database::setRoot(DatabaseObject *object) {
      root_ = object->dbref();
    }

    DatabaseObject* Database::root() {
      return findObjectByDbref(root_);
    }


  }  // namespace database
}  // namespace omush
