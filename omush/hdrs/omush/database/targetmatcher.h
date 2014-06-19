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
        std::vector<DatabaseObject*> aliasResponse;
        std::vector<DatabaseObject*> partialResponse;

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

        const boost::regex esc("[\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\]");
        const std::string rep("\\\\\\1&");
        target = regex_replace(target, esc, rep, boost::match_default | boost::format_sed);

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
            bool partial = true;
            database::DatabaseAttribute aliasAttr = iter.second->getAttribute("alias");
            std::string alias = aliasAttr.value;
            std::cout << "AliasL: " << alias << std::endl;

            std::vector<std::string> aliasParts;
            if (alias.length() > 0) {
              aliasParts = splitStringIntoSegments(alias, ";", 50);
              BOOST_FOREACH(std::string a, aliasParts) {
                if (partial == false)
                  continue;
                boost::trim(a);
                if (boost::iequals(a,target)) {
                  std::cout << "Add alias: " << a << std::endl;
                  aliasResponse.push_back(iter.second);
                  partial = false;
                }
              }
              //              response += fe.strParse(desc);
            }

            if (partial == false)
              continue;

            std::string::const_iterator start, end;
            start = name.begin();
            end = name.end();
            boost::match_results<std::string::const_iterator> what;
            boost::match_flag_type flags = boost::match_default;

            if (regex_search(start, end, what, exp, flags)) {
              std::cout << "Add partial: " << name << std::endl;
              partialResponse.push_back(iter.second);
            }

          }
        }

        if (!response.empty()) {
          return response;
        }
        else if(!aliasResponse.empty()) {
          return aliasResponse;
        }

        return partialResponse;
      }
    };

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_TARGETMATCHER_H_
