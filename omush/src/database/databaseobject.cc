#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    Dbref DatabaseObject::dbref() {
      return dbref_;
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

    void DatabaseObject::home(Dbref dbref) {
      home_ = dbref;
    }

    DbObjectType DatabaseObject::type() { return type_; }

    Dbref DatabaseObject::location() { return location_; }
    Dbref DatabaseObject::home() { return home_; }

    std::vector<Dbref> DatabaseObject::contents() { return contents_; }

    void DatabaseObject::removeFromContents(Dbref dbref) {
      std::vector<Dbref>::iterator position = std::find(contents_.begin(),
                                                        contents_.end(),
                                                        dbref);
      if (position == contents_.end())
        return;

      contents_.erase(position);
    }

    void DatabaseObject::addToContents(Dbref dbref) {
      if (std::find(contents_.begin(),
                    contents_.end(),
                    dbref) == contents_.end()) {
        contents_.push_back(dbref);
      }
    }
  }
}
