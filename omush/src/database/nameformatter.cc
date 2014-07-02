/**
 * \file nameformatter.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/nameformatter.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include "omush/database/databaseobject.h"
#include "omush/utility.h"
#include "omush/colorstring.h"
#include "omush/database/helpers.h"
#include <boost/foreach.hpp>
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    NameFormatter::NameFormatter(Database& db, DatabaseObject *looker)
      : db_(db), looker_(looker), returnDbref_(true) {
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
        color = "green";
        break;
      case database::DbObjectTypeDivision:
        color = "yellow";
        break;
      case database::DbObjectTypeThing:
      default:
        color = "blue";
        break;
      }
      std::string output = formatInline(object);

      if (returnDbref_) {
        output += std::string("(")
          + formatDbrefWithFlags(object->dbref())
          + std::string(")");
      }

      return ColorString::color(output, color);
    }

    NameFormatter& NameFormatter::noDbref() {
      returnDbref_ = false;
      return *this;
    }

    std::string NameFormatter::formatDbref(database::Dbref dbref) {
      std::string response = "#" + boost::lexical_cast<std::string>(dbref);

      return response;
    }

    std::string NameFormatter::formatDbrefWithFlags(database::Dbref dbref) {
      std::string response = formatDbref(dbref);

      database::DatabaseObject* object = dbrefToObject(db_, dbref);
      if (object == NULL)
        return response;

      switch (object->type()) {
      case DbObjectTypeRoom:
        response += "R";
        break;
      case DbObjectTypeExit:
        response += "E";
        break;
      case DbObjectTypePlayer:
        response += "P";
        break;
      case DbObjectTypeThing:
        response += "T";
        break;
      default:
        response += "U";
      }
      std::map<std::string,Flag> allFlags = db_.flags.getAllFlags();

      for (std::map<std::string,Flag>::iterator it = allFlags.begin();
           it != allFlags.end();
           ++it) {
        if (object->hasFlagByBit(it->second.bit)) {
          response += it->second.letter;
          continue;
        }
      }

      return response;
    }
  }  // namespace database
}  // namespace omush
