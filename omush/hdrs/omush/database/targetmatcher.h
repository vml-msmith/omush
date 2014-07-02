/**
 * \file targetmatcher.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_TARGETMATCHER_H_
#define OMUSH_HDRS_OMUSH_DATABASE_TARGETMATCHER_H_

#include "omush/database/definitions.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include <boost/algorithm/string/case_conv.hpp>
#include "omush/database/utility.h"
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include "omush/utility.h"

namespace omush {
  namespace database {

    class TargetMatcher {
     private:
      Database* db_;
      DatabaseObject* enactor_;
      std::map<DbObjectType,bool> types_;
      uint64_t searchFlags_;
     public:
      TargetMatcher(Database* db, DatabaseObject* enactor);
      void type(DbObjectType type);

      bool matchType(DatabaseObject *obj);

      std::vector<DatabaseObject*> match(std::string target);

      void setSearchFlags(uint64_t searchFlags);
    };

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_TARGETMATCHER_H_
