///
/// database.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_DATABASE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_H_

#include <map>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace omush {
  namespace database {
    class Database;
    enum DatabaseObjectType {
      DBGarbage,
      DBPlayer,
      DBThing,
      DBExit,
      DBRoom,
    };
    typedef int dbref;

    class DatabaseObject {
    public:
      DatabaseObjectType getType() { return type_; }
      dbref getDbref() { return dbref_; }
    protected:
      DatabaseObjectType type_;
      dbref dbref_;
      std::string name_;
    };


    class Player : public DatabaseObject {
    public:
      Player(std::string name, std::string pass, dbref r) { dbref_ = r; type_ = DBPlayer; name_ = name; password_ = pass; }
      static Player* findByNameAndPass(Database *db, std::string name, std::string pass);
      bool matchNameAndPass(std::string name, std::string pass) {
if (boost::iequals(name, name_) && boost::equals(pass,password_)) {
          std::cout << "MATCH (" << name_ << ") " << pass << std::endl;
return true;
        }
 return false;
      }
   private:
      std::string password_;
    };
    class Thing : public DatabaseObject {
    };
    class Exit : public DatabaseObject {
    };
    class Room : public DatabaseObject {
    };


    class Database {
      friend class Player;
     public:
      Database();
      void registerObject(DatabaseObject* db_object);
      DatabaseObject* getObjectByDbref(dbref ref);

    private:
      dbref highRef_;
      typedef std::map<dbref, DatabaseObject*> DBObjectMap;
      DBObjectMap allObjects_;
      std::map<DatabaseObjectType, DBObjectMap> objectsByType_;
    };
  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_H_
