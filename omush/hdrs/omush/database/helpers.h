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
//#include <boost/algorithm/string/case_conv.hpp>
//#include "omush/database/utility.h"
//#include <boost/foreach.hpp>
//#include "omush/utility.h"

namespace omush {
  namespace database {

    DatabaseObject* dbrefToObject(Database& db, Dbref dbref);

    DatabaseObject* objectLocation(Database& db, DatabaseObject* object);
    DatabaseObject* objectOwner(Database& db, DatabaseObject* object);

    std::string objectTypeString(DatabaseObject* object);

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_HELPERS_H_
