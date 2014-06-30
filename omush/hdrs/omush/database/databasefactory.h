/**
 * \file databasefactory.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DATABASEFACTORY_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DATABASEFACTORY_H_
#include "omush/database/database.h"

namespace omush {
  namespace database {
    class DatabaseFactory {
     public:
      static Database* createDatabase();
      static void setupFlags(Database* db);
      static void setupPowers(Database* db);
      static void setupAttributes(Database* db);
      static void defaultObjects(Database* db);
      static void loadObjectsFromDatabaseFile(Database* db,
                                              std::string name);
     private:
      DatabaseFactory() {}
    };
  }  // database
}  // omush

#endif //  OMUSH_HDRS_OMUSH_DATABASE_DATABASEFACTORY_H_
