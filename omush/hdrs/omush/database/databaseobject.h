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

//#include <boost/algorithm/string.hpp>
//#include <map>
//#include <vector>

namespace omush {
  namespace database {

    struct DatabaseAttribute {
      std::map<std::string,DatabaseAttribute> tree;
      std::string value;
    };

    class DatabaseObject {
      friend class DatabaseObjectFactory;
      friend class Database;
     public:
      Dbref ref();
      std::string getProperty(std::string propertyName);
      DatabaseAttribute getAttribute(std::string name);
      void setProperty(std::string name, std::string value);
      void setAttribute(std::string name, std::string value);
      DbObjectType type();
      Dbref location();
      std::vector<Dbref> contents();

     protected:
      typedef std::map<std::string,std::string> PropertyMap;
      typedef std::map<std::string,DatabaseAttribute> AttributeMap;
      Dbref ref_;
      PropertyMap properties_;
      AttributeMap attributes_;
      DbObjectType type_;
      Dbref location_;
      std::vector<Dbref> contents_;
    };
  }
}

#endif // OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECT_H_
