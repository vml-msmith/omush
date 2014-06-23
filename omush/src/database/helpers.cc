/**
 *
 */
#include "omush/database/helpers.h"
#include "omush/power.h"

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

    bool hasQuota(Database& db, DatabaseObject* object, int quota) {
      if (hasPower(db, object, "unlimited quota"))
        return true;

      return (object->getAvailableQuota() - quota) >= 0;
    }

    bool hasCredit(DatabaseObject* object, int credit) {
      // Has power unlimited credit.

      return (object->getAvailableCredit() - credit) >= 0;
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

    bool hasPower(Database& db, DatabaseObject* object, std::string name) {
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return false;
      }

      return object->hasPowerByBit(p->bit);
    }
  }
}
