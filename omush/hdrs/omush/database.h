///
/// database.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_DATABASE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_H_

#include <map>

namespace omush {
  enum DatabaseObjectType {
    DBPlayer,
    DBThing,
    DBExit,
    DBRoom,
  };
  typedef int dbref;
  class DatabaseObject {
  };

  class Player : public DatabaseObject {
  };
  class Thing : public DatabaseObject {
  };
  class Exit : public DatabaseObject {
  };
  class Room : public DatabaseObject {
  };


  class Database {
   public:
    void registerObject(DatabaseObject db_object) {
      // add to all by type.
      // Add to all.
      // Increment ref.
    }
  private:
    typedef std::map<dbref, DatabaseObject*> DBObjectMap;
    DBObjectMap allObjects_;
    std::map<DatabaseObjectType, DBObjectMap> objectsByType_;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_H_
