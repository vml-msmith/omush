/**
 * \file playerutilities.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_PLAYERUTILITIES_H_
#define OMUSH_HDRS_OMUSH_DATABASE_PLAYERUTILITIES_H_

#include <string>
#include "omush/database/definitions.h"

namespace omush {
  namespace database {
    class PlayerUtilities {
     public:
      static DatabaseObject* findPlayerByName(Database *db, std::string name);
    };
  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_PLAYERUTILITIES_H_
