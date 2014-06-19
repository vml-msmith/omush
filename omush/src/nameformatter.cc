/**
 * \file nameformatter.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/nameformatter.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include "omush/database/databaseobject.h"
#include "omush/utility.h"

namespace omush {
  NameFormatter::NameFormatter(database::DatabaseObject *looker)
    : looker_(looker), returnDbref_(true) {
  }

  std::string NameFormatter::formatInline(database::DatabaseObject *object) {
    if (object == NULL) {
      return "unknown";
    }
    std::string output = object->getProperty("name");
    return output;
  }

  std::string NameFormatter::format(database::DatabaseObject *object) {
    if (object == NULL) {
      return "unknown";
    }

    std::string color = "";
    switch (object->type()) {
    case database::DbObjectTypePlayer:
      color = "red";
      break;
    case database::DbObjectTypeExit:
      color = "cyan";
      break;
    case database::DbObjectTypeRoom:
    default:
      color = "green";
      break;
    }
    std::string output = formatInline(object);

    if (returnDbref_) {
        output += std::string("(")
          + formatDbref(object->dbref())
          + std::string(")");
    }

    return ColorString::color(output, color);
  }

  NameFormatter& NameFormatter::noDbref() {
    returnDbref_ = false;
    return *this;
  }

  std::string NameFormatter::formatDbref(database::Dbref dbref) {
    return "#" + boost::lexical_cast<std::string>(dbref);
  }
}  // namespace omush
