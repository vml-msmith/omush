/**
 * \file utilityfactories.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_UTILITYFACTORIES_H_
#define OMUSH_HDRS_OMUSH_DATABASE_UTILITYFACTORIES_H_

#include "omush/database/targetmatcher.h"
#include "omush/database/nameformatter.h"

namespace omush {
  namespace database {
    TargetMatcher targetMatcher(Database* db, DatabaseObject* looker);
    NameFormatter nameFormatter(Database& db, DatabaseObject* looker);
  }  // namespace database
} // omush

#endif //  OMUSH_HDRS_OMUSH_DATABASE_UTILITYFACTORIES_H_
