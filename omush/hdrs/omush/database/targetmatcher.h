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

namespace omush {
  namespace database {

    class TargetMatcher {
     public:
      static std::vector<DatabaseObject*> match(Database* db,
                                                DatabaseObject* enactor,
                                                std::string target) {
        std::vector<DatabaseObject*> response;

        if (boost::iequals(target, "ME")) {
          response.push_back(enactor);
          return response;
        }

        if (boost::iequals(target, "HERE")) {
          DatabaseObject *obj = db->findObjectByDbref(enactor->location());
          if (obj != NULL) {
            response.push_back(obj);
            return response;
          }
        }

        if (database::Utility::isDbref(target)) {
          Dbref dbref = database::Utility::parseDbrefFromString(target);

          DatabaseObject *obj = db->findObjectByDbref(dbref);
          if (obj != NULL) {
            response.push_back(obj);
            return response;
          }
        }

        DbMap objs = Utility::getSurroundings(db, enactor);
        const boost::regex exp(target, boost::regex::icase);
        bool searchPartial = true;
        BOOST_FOREACH (const DbMap::value_type& iter, objs) {

          std::string name = iter.second->getProperty("name");
          if (boost::iequals(name, target)) {
            searchPartial = false;
            response.push_back(iter.second);
          }
          else if (searchPartial) {
            std::string::const_iterator start, end;
            start = name.begin();
            end = name.end();
            boost::match_results<std::string::const_iterator> what;
            boost::match_flag_type flags = boost::match_default;

            if (regex_search(start, end, what, exp, flags)) {
              response.push_back(iter.second);
            }
          }
        }

        return response;;
      }
    };

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_TARGETMATCHER_H_
