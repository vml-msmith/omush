/**
 * \file utility.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_UTILITY_H_
#define OMUSH_HDRS_OMUSH_DATABASE_UTILITY_H_

#include <map>
#include "omush/database/definitions.h"
#include "omush/database/databaseobject.h"
#include "omush/database/database.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace omush {
  namespace database {
    #define SEARCH_LOCATION   0x10U
    #define SEARCH_INVENTORY  0x20U
    #define SEARCH_SELF       0x40U
    #define SEARCH_REMOTE     0x80U

    class Utility {
    public:
      static bool isDbref(std::string str) {
        static const boost::regex e("#[0-9]+");
        return regex_match(str, e);
      }
      static Dbref parseDbrefFromString(std::string str) {
        return boost::lexical_cast<Dbref>(str.substr(1, str.length()));
      }
      static DbMap getSurroundings(database::Database* db,
                                   database::DatabaseObject *object) {
        return getSurroundings(db,
                               object,
                               0 | SEARCH_LOCATION | SEARCH_SELF);
      }
      static DbMap getSurroundings(database::Database* db,
                                   database::DatabaseObject *object,
                                   uint64_t locationFlags) {
        DbMap listeners;
        if ((locationFlags & SEARCH_SELF) == SEARCH_SELF)
          listeners[object->dbref()] = object;

        if ((locationFlags & SEARCH_LOCATION) == SEARCH_LOCATION) {
          database::DatabaseObject *location = db->findObjectByDbref(object->location());
          if (location != NULL)  {

            listeners[location->dbref()] = location;

            // Add the location's contents.
            std::vector<database::Dbref> contents = location->contents();
            for (std::vector<database::Dbref>::iterator iter = contents.begin();
                 iter != contents.end();
                 ++iter) {
              database::DatabaseObject *obj = db->findObjectByDbref(*iter);
              if (obj != NULL) {
              listeners[obj->dbref()] = obj;
              }
            }
          }
        }

        if ((locationFlags & SEARCH_INVENTORY) == SEARCH_INVENTORY) {
          // Add the contents of the item.
          std::vector<database::Dbref> inventory = object->contents();
          for (std::vector<database::Dbref>::iterator iter = inventory.begin();
               iter != inventory.end();
               ++iter) {
            database::DatabaseObject *obj = db->findObjectByDbref(*iter);
            if (obj != NULL) {
              listeners[obj->dbref()] = obj;
            }
          }
        }
        return listeners;
      }

    };
  }
}

#endif
