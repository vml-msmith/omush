/**
 * \file nameformatter.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NAMEFORMATTER_H_
#define OMUSH_HDRS_OMUSH_NAMEFORMATTER_H_

#include <string>
#include "omush/database/databaseobject.h"

namespace omush {
  class NameFormatter {
   public:
    explicit NameFormatter(database::DatabaseObject *looker);

    std::string formatInline(database::DatabaseObject *object);

    std::string format(database::DatabaseObject *object);

    NameFormatter& noDbref();

    std::string formatDbref(database::Dbref dbref);

   private:
    bool returnDbref_;
    database::DatabaseObject *looker_;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NAMEFORMATTER_H_
