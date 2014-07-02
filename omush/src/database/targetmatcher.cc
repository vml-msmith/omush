/**
 * \file targetmatcher.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/targetmatcher.h"

namespace  omush {
  namespace database {
    TargetMatcher::TargetMatcher(Database* db, DatabaseObject* enactor) {
      db_ = db;
      enactor_ = enactor;
      searchFlags_ = SEARCH_LOCATION | SEARCH_INVENTORY | SEARCH_SELF | SEARCH_REMOTE;
    }

    void TargetMatcher::type(DbObjectType type) {
      types_[type] = true;
    }

    void TargetMatcher::setSearchFlags(uint64_t searchFlags) {
      searchFlags_ = searchFlags;
    }

    bool TargetMatcher::matchType(DatabaseObject *obj) {
      if (types_.size() == 0)
        return true;
      if (types_.find(obj->type()) != types_.end())
        return true;

      return false;
    }

    std::vector<DatabaseObject*> TargetMatcher::match(std::string target) {
      std::vector<DatabaseObject*> response;
      std::vector<DatabaseObject*> aliasResponse;
      std::vector<DatabaseObject*> partialResponse;

      if (boost::iequals(target, "ME") &&
          matchType(enactor_) &&
          ((searchFlags_ & SEARCH_SELF) == SEARCH_SELF)) {
        response.push_back(enactor_);
        return response;
      }

      DatabaseObject *loc = db_->findObjectByDbref(enactor_->location());
      if (loc != NULL &&
          boost::iequals(target, "HERE") &&
          matchType(loc) &&
          ((searchFlags_ & SEARCH_LOCATION) == SEARCH_LOCATION)) {
        response.push_back(loc);
        return response;
      }

      if (database::Utility::isDbref(target)) {
        Dbref dbref = database::Utility::parseDbrefFromString(target);
        DatabaseObject *obj = db_->findObjectByDbref(dbref);
        if (obj != NULL && matchType(obj)) {
          DatabaseObject *objLoc = db_->findObjectByDbref(obj->location());
          if (((searchFlags_ & SEARCH_LOCATION) == SEARCH_REMOTE) ||
              (objLoc == enactor_ || objLoc == loc))  {
            response.push_back(obj);
            return response;
          }
        }
      }

      DbMap objs = Utility::getSurroundings(db_, enactor_, searchFlags_);

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

  }  // namespace database
}  // namespace  omush
