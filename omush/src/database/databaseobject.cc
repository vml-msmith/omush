#include "omush/database/databaseobject.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace omush {
  namespace database {
    DatabaseObject::DatabaseObject() :
      password_(""), flags_(0) {
      for (int i = 0; i <= 5; ++i)
        powers_.push_back(0);
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

    int DatabaseObject::getPropertyInt(std::string propertyName) {
      try {
        return boost::lexical_cast<int>(getProperty(propertyName));
      } catch (boost::bad_lexical_cast&) {
        return 0;
      }
    }

    int DatabaseObject::getQuotaLimit() {
      return getPropertyInt("quotaLimit");
    }
    int DatabaseObject::getCreditLimit() {
      return getPropertyInt("creditLimit");
    }
    int DatabaseObject::getQuota() {
      return getPropertyInt("quota");
    }
    int DatabaseObject::getCredit() {
      return getPropertyInt("credit");
    }
    int DatabaseObject::getAvailableQuota() {
      return getQuotaLimit() - getQuota();
    }
    int DatabaseObject::getAvailableCredit() {
      return getCredit();
    }
    void DatabaseObject::addCredit(int credit) {
      setPropertyInt("credit", getCredit() + credit);
    }
    void DatabaseObject::addQuota(int quota) {
      setPropertyInt("quota", getQuota() + quota);
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

    void DatabaseObject::setPropertyInt(std::string name, int value) {
      std::string val = "";
      try {
        val = boost::lexical_cast<std::string>(value);
      } catch (boost::bad_lexical_cast&) {
        val = "0";
      }

      setProperty(name, val);
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

    void DatabaseObject::setOwner(DatabaseObject* obj) {
      owner_ = obj->dbref();
    }

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

    void DatabaseObject::setPassword(std::string str) {
      password_ = str;
    }

    bool DatabaseObject::hasFlagByBit(uint64_t bit) {
      return (flags_ & bit) == bit;
    }


    bool DatabaseObject::hasPowerByBit(uint32_t bit) {
      return hasPowerByBit(bit, 0);
    }

    bool DatabaseObject::hasPowerByBit(uint32_t bit, int level) {
      if (level < 0 || level > 5)
        return false;

      return (powers_[level] & bit) == bit;
    }

    void DatabaseObject::addPowerByBit(uint32_t bit) {
      addPowerByBit(bit, 0);
    }


    void DatabaseObject::addPowerByBit(uint32_t bit, int level) {
      if (level < 0 || level > 5)
        return;

      powers_[level] = (powers_[level] | bit);

      if (level != 0)
        powers_[0] = (powers_[0] | bit);
    }

    void DatabaseObject::removePowerByBit(uint32_t bit) {
      if (hasPowerByBit(bit)) {
        powers_[0] = (powers_[0] ^ bit);
      }
    }
  }
}
