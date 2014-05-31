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

    struct DatabaseAttribute {
      AttributeMap tree;
      std::string value;
    };

    class DatabaseObject {
      friend class DatabaseObjectFactory;
      friend class Database;
     public:
      Dbref dbref();
      std::string getProperty(std::string propertyName);
      DatabaseAttribute getAttribute(std::string name);
      void setProperty(std::string name, std::string value);
      void setAttribute(std::string name, std::string value);
      DbObjectType type();
      Dbref location();
      std::vector<Dbref> contents();

      void removeFromContents(Dbref dbref);
      void addToContents(Dbref dbref);
      void home(Dbref dbref);

     protected:
      typedef std::map<std::string,std::string> PropertyMap;
      typedef std::map<std::string,DatabaseAttribute> AttributeMap;
      Dbref dbref_;
      PropertyMap properties_;
      AttributeMap attributes_;
      DbObjectType type_;
      Dbref location_;
      Dbref home_;
      std::vector<Dbref> contents_;
    };
  }  // namespace database
}  // namespace omush

#endif // OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECT_H_
