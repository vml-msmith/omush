/**
 *
 */
#include "omush/database/helpers.h"

namespace omush {
  namespace database {
    DatabaseObject* dbrefToObject(Database& db, Dbref dbref) {
      return db.findObjectByDbref(dbref);
    }

    DatabaseObject* objectLocation(Database& db, DatabaseObject* object) {
      Dbref loc = object->location();
      return dbrefToObject(db, loc);
    }

    DatabaseObject* objectOwner(Database& db, DatabaseObject* object) {
      Dbref loc = object->owner();
      return dbrefToObject(db, loc);
    }

    std::string objectTypeString(DatabaseObject* object) {
      switch(object->type()) {
      case database::DbObjectTypePlayer:
        return "Player";
        break;
      case database::DbObjectTypeExit:
        return "Exit";
        break;
      case database::DbObjectTypeRoom:
        return "Room";
        break;
      case database::DbObjectTypeThing:
        return "Thing";
        break;
      }
      return "Unknown";
    }
  }
}
