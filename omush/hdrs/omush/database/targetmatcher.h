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
     private:
      Database* db_;
      DatabaseObject* enactor_;
      std::map<DbObjectType,bool> types_;
     public:
      TargetMatcher(Database* db, DatabaseObject* enactor) {
        db_ = db;
        enactor_ = enactor;
      }

      void type(DbObjectType type) {
        types_[type] = true;
      }

      bool matchType(DatabaseObject *obj) {
        if (types_.size() == 0)
          return true;
        if (types_.find(obj->type()) != types_.end())
          return true;

        return false;
      }

      std::vector<DatabaseObject*> match(std::string target) {
        std::vector<DatabaseObject*> response;

        if (boost::iequals(target, "ME") && matchType(enactor_)) {
          response.push_back(enactor_);
          return response;
        }

        if (boost::iequals(target, "HERE")) {
          DatabaseObject *obj = db_->findObjectByDbref(enactor_->location());
          if (obj != NULL && matchType(obj)) {
            response.push_back(obj);
            return response;
          }
        }

        if (database::Utility::isDbref(target)) {
          Dbref dbref = database::Utility::parseDbrefFromString(target);

          DatabaseObject *obj = db_->findObjectByDbref(dbref);
          if (obj != NULL && matchType(obj)) {
            response.push_back(obj);
            return response;
          }
        }

        DbMap objs = Utility::getSurroundings(db_, enactor_);
        const boost::regex exp(target, boost::regex::icase);
        bool searchPartial = true;
        BOOST_FOREACH (const DbMap::value_type& iter, objs) {
          if (!matchType(iter.second)) {
            continue;
          }
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
