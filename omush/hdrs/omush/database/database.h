/**
 * \file database.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_

#include "omush/database/definitions.h"
#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>

namespace omush {
  namespace database {


typedef std::map<Dbref,DatabaseObject*> DbMap;
typedef std::map<DbObjectType,DbMap> DbTypeMap;

    class PlayerUtilities;
    class Database {
      friend class PlayerUtilities;
    public:
      Database() : top_(0) {}
      void addObject(DatabaseObject* obj);
      Dbref getNextRef();
      void moveObject(DatabaseObject* obj, Dbref ref);
      DatabaseObject* findObjectByDbref(Dbref ref);
    private:
      DbMap allObjects_;
      DbTypeMap typedObjects_;
      Dbref top_;
    };

    class DatabaseObjectFactory {
    public:
      static DatabaseObject* createPlayer(Database* db);
      static DatabaseObject* createRoom(Database* db);
    };

    class PlayerUtilities {
    public:
      static DatabaseObject* findPlayerByName(Database *db, std::string name);
    };
  }
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_
