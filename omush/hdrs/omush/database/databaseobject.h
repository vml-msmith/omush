/**
 * \file databaseobject.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECT_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECT_H_
#include "omush/database/definitions.h"

#include <string>
#include <vector>
#include <map>

namespace omush {
  namespace database {
    typedef std::map<std::string, DatabaseAttribute> AttributeMap;
    typedef std::map<std::string,std::string> PropertyMap;
    typedef std::map<std::string,bool> AliasMap;

    struct DatabaseAttribute {
      AttributeMap tree;
      std::string value;
      DatabaseAttribute() : value("") {}
    };

    class DatabaseObject {
      friend class DatabaseObjectFactory;
      friend class Database;
     public:
      explicit DatabaseObject();
      Dbref dbref();
      std::string getProperty(std::string propertyName);
      int getPropertyInt(std::string propertyName);
      DatabaseAttribute getAttribute(std::string name);
      void setProperty(std::string name, std::string value);
      void setPropertyInt(std::string name, int value);
      void setAttribute(std::string name, std::string value);
      DbObjectType type();
      Dbref location();
      std::vector<Dbref> contents();

      void removeFromContents(Dbref dbref);
      void addToContents(Dbref dbref);
      void home(Dbref dbref);
      Dbref home();
      Dbref owner();
      void setOwner(DatabaseObject* obj);
      AttributeMap attributes();
      PropertyMap properties();

      void printAttrs();

      bool comparePassword(std::string str);
      void setPassword(std::string str);


      int getQuotaLimit();
      int getCreditLimit();
      int getQuota();
      int getCredit();
      int getAvailableQuota();
      int getAvailableCredit();
      void addCredit(int credit);
      void addQuota(int quota);

      bool hasFlagByBit(uint64_t bit);
      void addFlagByBit(uint64_t bit);
      void removeFlagByBit(uint64_t bit);

      bool hasPowerByBit(uint64_t bit);
      void addPowerByBit(uint64_t bit);
      void removePowerByBit(uint64_t bit);
      bool hasPowerByBit(uint64_t bit, int level);
      void addPowerByBit(uint64_t bit, int level);

     protected:
      typedef std::map<std::string,std::string> PropertyMap;
      typedef std::map<std::string,DatabaseAttribute> AttributeMap;
      Dbref dbref_;
      PropertyMap properties_;
      AttributeMap attributes_;
      DbObjectType type_;
      Dbref location_;
      Dbref home_;
      Dbref owner_;
      std::string password_;
      std::vector<Dbref> contents_;
      uint64_t flags_;
      std::vector<uint64_t> powers_;

    };
  }  // namespace database
}  // namespace omush

#endif // OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECT_H_
