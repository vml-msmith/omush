/**
 *
 */
#include "omush/database/helpers.h"
#include "omush/power.h"
#include <iostream>

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

    bool hasCredit(Database& db, DatabaseObject* object, int credit) {
     if (hasPower(db, object, "unlimited credits"))
        return true;

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
      // Add provision to give God all the powers, just in case.
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return false;
      }

      return object->hasPowerByBit(p->bit);
    }

    void addPower(Database& db, DatabaseObject* object, std::string name) {
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return;
      }

      return object->addPowerByBit(p->bit);
    }

    void addPowerByBit(DatabaseObject* object, uint32_t bit) {
      return object->addPowerByBit(bit);
    }

    void removePowerByBit(DatabaseObject* object, uint32_t bit) {
      return object->removePowerByBit(bit);
    }

    void removePower(Database& db, DatabaseObject* object, std::string name) {
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return;
      }

      return object->removePowerByBit(p->bit);
    }
  }
}
