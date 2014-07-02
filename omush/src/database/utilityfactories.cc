/**
 * \file utilityfactories.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/utilityfactories.h"

namespace omush {
  namespace database {
    TargetMatcher targetMatcher(Database* db, DatabaseObject* looker) {
      return TargetMatcher(db, looker);
    }
    NameFormatter nameFormatter(Database& db, DatabaseObject* looker) {
      return NameFormatter(db, looker);
    }
  }  // namespace database
} // omush
