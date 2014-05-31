/**
 * \file nameformatter.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NAMEFORMATTER_H_
#define OMUSH_HDRS_OMUSH_NAMEFORMATTER_H_

#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/utility.h"
#include <boost/algorithm/string.hpp>
#include "omush/notifier.h"
#include <vector>
#include <boost/lexical_cast.hpp>

namespace omush {

  class NameFormatter {
  public:
    NameFormatter(database::DatabaseObject *looker) : looker_(looker) {
    }

    std::string formatInline(database::DatabaseObject *object) {
      std::string output = object->getProperty("name");
      return output;
    }

    std::string format(database::DatabaseObject *object) {
      std::string color = "";
      switch (object->type()) {
      case database::DbObjectTypePlayer:
        color = "red";
        break;
      case database::DbObjectTypeRoom:
      default:
        color = "green";
        break;
      }
      std::string output = formatInline(object);

      output += std::string("(")
        + formatDbref(object->dbref())
        + std::string(")");

      return ColorString::color(output, color);
    }
    std::string formatDbref(database::Dbref dbref) {
      return "#" + boost::lexical_cast<std::string>(dbref);
    }
  private:
    database::DatabaseObject *looker_;

  };

  class TargetMatcher {
    database::DatabaseObject* match(database::DatabaseObject* enactor,
                                   std::string match) {
      return NULL;
    }
  };
}

#endif
