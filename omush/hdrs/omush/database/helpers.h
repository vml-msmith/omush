/**
 * \file helpers.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_HELPERS_H_
#define OMUSH_HDRS_OMUSH_DATABASE_HELPERS_H_

#include "omush/database/definitions.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"

namespace omush {
  namespace database {

    DatabaseObject* dbrefToObject(Database& db, Dbref dbref);

    DatabaseObject* objectLocation(Database& db, DatabaseObject* object);
    DatabaseObject* objectOwner(Database& db, DatabaseObject* object);

    std::string objectTypeString(DatabaseObject* object);

    bool hasQuota(Database& db, DatabaseObject* object, int quota);

    bool hasCredit(DatabaseObject* object, int quota);

    bool hasPower(Database& db, DatabaseObject* object, std::string name);
    void  addPower(Database& db, DatabaseObject* object, std::string name);
    void removePower(Database& db, DatabaseObject* object, std::string name);


  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_HELPERS_H_
