#include "omush/database/databaseobject.h"
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace omush {
  namespace database {
    DatabaseObject::DatabaseObject() :
      password_("") {

    }
    Dbref DatabaseObject::dbref() {
      return dbref_;
    }

    void DatabaseObject::printAttrs() {
      for (AttributeMap::iterator i = attributes_.begin(); i != attributes_.end(); ++i) {
        std::cout << i->first << std::endl;
      }
    }

    AttributeMap DatabaseObject::attributes() {
      return attributes_;
    }

    PropertyMap DatabaseObject::properties() {
      return properties_;;
    }


    std::string DatabaseObject::getProperty(std::string propertyName) {
      boost::to_upper(propertyName);
      if (properties_.find(propertyName) == properties_.end()) {
        return "";
      }
      return properties_[propertyName];
    }

    DatabaseAttribute DatabaseObject::getAttribute(std::string name) {
      boost::to_upper(name);

      if (attributes_.find(name) == attributes_.end()) {
        return DatabaseAttribute();
      }

      return attributes_[name];
    }

    void DatabaseObject::setProperty(std::string name, std::string value) {
      boost::to_upper(name);
      properties_[name] = value;
    }

    void DatabaseObject::setAttribute(std::string name, std::string value) {
      boost::to_upper(name);
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
    Dbref DatabaseObject::owner() { return owner_; }

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

    bool DatabaseObject::comparePassword(std::string str) {
      return str == password_;
    }
  }
}
