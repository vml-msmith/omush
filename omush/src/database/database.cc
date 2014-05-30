#include "omush/database.h"
#include <map>

namespace omush {
  namespace database {
    dbref DatabaseObject::ref() {
      return ref_;
    }

    std::string DatabaseObject::getProperty(std::string propertyName) {
      if (properties_.find(propertyName) == properties_.end()) {
        return "";
      }
      return properties_[propertyName];
    }

    DatabaseAttribute DatabaseObject::getAttribute(std::string name) {
      if (attributes_.find(name) == attributes_.end()) {
        return DatabaseAttribute();
      }
      return attributes_[name];
    }

    void DatabaseObject::setProperty(std::string name, std::string value) {
      properties_[name] = value;
    }

    void DatabaseObject::setAttribute(std::string name, std::string value) {
      if (attributes_.find(name) == attributes_.end()) {
        attributes_[name] = DatabaseAttribute();
      }

      attributes_[name].value = value;
    }

    DbObjectType DatabaseObject::type() { return type_; }

    dbref DatabaseObject::location() { return location_; }
    std::vector<dbref> DatabaseObject::contents() { return contents_; }

    DatabaseObject* Database::findObjectByDbref(dbref ref) {
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

    void Database::moveObject(DatabaseObject* obj, dbref ref) {
      DatabaseObject *newLocation = findObjectByDbref(ref);
std::cout << "Move to " << ref << std::endl;
      if (newLocation == NULL) {
std::cout << "Fail" << std::endl;
        obj->location_ = -1;
        return;
      }

      obj->location_ = ref;
      if (obj->type_ != DbObjectTypeRoom &&
            std::find(newLocation->contents_.begin(),
                        newLocation->contents_.end(),
                        ref) == newLocation->contents_.end()) {
std::cout << "Add it" << std::endl;
newLocation->contents_.push_back(obj->ref());
      }
    }

    dbref Database::getNextRef() {
      return top_;
    }


  }
}
