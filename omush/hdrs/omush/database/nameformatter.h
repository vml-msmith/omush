/**
 * \file nameformatter.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_NAMEFORMATTER_H_
#define OMUSH_HDRS_OMUSH_DATABASE_NAMEFORMATTER_H_

#include <string>
#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    class NameFormatter {
     public:
      explicit NameFormatter(Database& db, database::DatabaseObject *looker);

      std::string formatInline(database::DatabaseObject *object);

      std::string format(database::DatabaseObject *object);

      NameFormatter& noDbref();

      std::string formatDbref(database::Dbref dbref);
      std::string formatDbrefWithFlags(database::Dbref dbref);
     private:
      bool returnDbref_;
      database::DatabaseObject *looker_;
      Database& db_;
    };
  }  // namespace database

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_NAMEFORMATTER_H_
