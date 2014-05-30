#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    Dbref DatabaseObject::ref() {
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

    Dbref DatabaseObject::location() { return location_; }
    std::vector<Dbref> DatabaseObject::contents() { return contents_; }
  }
}
