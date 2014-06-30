/**
 * \file databaseobjectfactory.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECTFACTORY_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECTFACTORY_H_

#include <boost/algorithm/string.hpp>
#include <map>
#include "omush/database/definitions.h"

namespace omush {
  namespace database {
    class DatabaseObjectFactory {
     public:
      static std::map<std::string,DbObjectType> types;

      static DatabaseObject* createObject(Database* db,
                                          std::string type);
      static DatabaseObject* createObject(Database* db,
                                          std::string type,
                                          Dbref dbref);
      static DatabaseObject* createPlayer(Database* db);
      static DatabaseObject* createPlayer(Database* db, Dbref dbref);
      static DatabaseObject* createRoom(Database* db);
      static DatabaseObject* createRoom(Database* db, Dbref dbref);
      static DatabaseObject* createExit(Database* db);
      static DatabaseObject* createExit(Database* db, Dbref dbref);
      static DatabaseObject* createThing(Database* db);
      static DatabaseObject* createThing(Database* db, Dbref dbref);
    };

  }  // namespace database
}  // namespace omush


#endif  // OMUSH_HDRS_OMUSH_DATABASE_DATABASEOBJECTFACTORY_H_
