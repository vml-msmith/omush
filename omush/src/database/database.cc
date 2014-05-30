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

    void Database::addObject(DatabaseObject* obj) {
      if (obj->ref() >= top_) {
        ++top_;
      }

      allObjects_.insert(std::make_pair(obj->ref(), obj));
      typedObjects_[obj->type_].insert(std::make_pair(obj->ref(), obj));
      // Move it to it's new home.
      moveObject(obj, obj->location_);
    }

    void Database::moveObject(DatabaseObject* obj, Dbref ref) {
      DatabaseObject *newLocation = findObjectByDbref(ref);
      if (newLocation == NULL) {
        obj->location_ = -1;
        return;
      }

      obj->location_ = ref;
      if (obj->type_ != DbObjectTypeRoom &&
          std::find(newLocation->contents_.begin(),
                    newLocation->contents_.end(),
                    ref) == newLocation->contents_.end()) {
        newLocation->contents_.push_back(obj->ref());
      }
    }

    Dbref Database::getNextRef() {
      return top_;
    }


  }  // namespace database
}  // namespace omush
