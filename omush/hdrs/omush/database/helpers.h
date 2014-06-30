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
    bool hasCredit(Database& db, DatabaseObject* object, int quota);

    bool hasPowerOver(Database& db,
                      DatabaseObject* object,
                      DatabaseObject* target,
                      std::string name);
    bool hasPower(Database& db, DatabaseObject* object, std::string name);
    bool hasPower(Database& db, DatabaseObject* object, std::string name, int level);
    void addPower(Database& db, DatabaseObject* object, std::string name);
    void addPower(Database& db, DatabaseObject* object, std::string name, int level);
    void addPowerByBit(DatabaseObject* object, uint32_t bit);
    void addPowerByBit(DatabaseObject* object, uint32_t bit, int level);
    void removePower(Database& db, DatabaseObject* object, std::string name);
    void removePowerByBit(DatabaseObject* object, uint32_t bit);

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_HELPERS_H_
