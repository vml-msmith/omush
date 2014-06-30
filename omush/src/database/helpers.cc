/**
 *
 */
#include "omush/database/helpers.h"
#include "omush/power.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace omush {
  namespace database {
    DatabaseObject* dbrefToObject(Database& db, Dbref dbref) {
      return db.findObjectByDbref(dbref);
    }

    Dbref parseStringDbref(std::string dbref) {
      std::cout << "Parse stirng: " << dbref << std::endl;
      if (dbref.length() < 2)
        return -1;
      std::cout << "PArse paset -1" << std::endl;
      if (dbref[0] != '#')
        return -1;
      std::cout << "Parse past #" << std::endl;
      int response;
      try {
        std::cout << "Lexical cast" << std::endl;
        response = boost::lexical_cast<int>(dbref.substr(1,dbref.length()));
      } catch (boost::bad_lexical_cast&) {
        std::cout << "Excet" << std::endl;
        return -1;
      }
      return response;
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


    bool hasPowerOver(Database& db,
                      DatabaseObject* object,
                      DatabaseObject* target,
                      std::string name) {
      std::vector<bool> powers;

      for (int i = 0; i <= 5; ++i) {
        powers.push_back(hasPower(db, object, name, i));
      }

      DatabaseObject* targetOwner = dbrefToObject(db, target->owner());
      DatabaseObject* targetEmpire = NULL;
      DatabaseObject* targetDivision = NULL;
      int targetClass = 0;
      if (targetOwner != NULL) {
        std::cout << "Target owner: " << targetOwner->getProperty("name") << std::endl;
        int targetEmpireDbref = parseStringDbref(targetOwner->getAttribute("empire").value);
        if (targetEmpireDbref >= 0) {
          std::cout << "Target empire: " << targetEmpireDbref << std::endl;
          targetEmpire = dbrefToObject(db, targetEmpireDbref);
        }
        int targetDivDbref = parseStringDbref(targetOwner->getAttribute("division").value);
        if (targetDivDbref >= 0) {
          targetDivision = dbrefToObject(db, targetDivDbref);
          std::cout << "Target div: " << targetDivDbref << std::endl;
        }
        try {
          targetClass = boost::lexical_cast<int>(targetOwner->getAttribute("classvalue").value);
        }
        catch (boost::bad_lexical_cast&) {}

      }

      DatabaseObject* objectOwner = dbrefToObject(db, object->owner());
      DatabaseObject* objectEmpire = NULL;
      DatabaseObject* objectDivision = NULL;
      int objectClass = 0;

      if (objectOwner != NULL) {
        std::cout << "object owner: " << objectOwner->getProperty("name") << std::endl;
        int objectEmpireDbref = parseStringDbref(objectOwner->getAttribute("empire").value);
        if (objectEmpireDbref >= 0)
          objectEmpire = dbrefToObject(db, objectEmpireDbref);
        int objectDivDbref = parseStringDbref(objectOwner->getAttribute("division").value);
        if (objectDivDbref >= 0)
          objectDivision = dbrefToObject(db, objectDivDbref);
        try {
          objectClass = boost::lexical_cast<int>(objectOwner->getAttribute("classvalue").value);
        }
        catch (boost::bad_lexical_cast&) {}
      }

      if (powers[1]) {
        if (objectClass > targetClass)
          return true;
        return false;
      }

      if (powers[2]) {
        if(targetEmpire == objectEmpire && objectEmpire != NULL)
          return true;
        return false;
      }

      if (powers[3]) {
        if (objectClass > targetClass && targetEmpire == objectEmpire && objectEmpire != NULL)
          return true;
        return false;
      }

      if (powers[4]) {
        if (targetDivision == objectDivision && objectDivision != NULL)
          return true;
        return false;
      }

      if (powers[5]) {
        if (objectClass > targetClass && targetDivision == objectDivision && objectDivision != NULL)
          return true;
        return false;
      }

      // Everyone, return true.
      if (powers[0])
        return true;

      return false;
    }

    bool hasPower(Database& db, DatabaseObject* object, std::string name) {
      return hasPower(db, object, name, 0);
    }

    bool hasPower(Database& db, DatabaseObject* object, std::string name, int level) {
      // Add provision to give God all the powers, just in case.
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return false;
      }

      return object->hasPowerByBit(p->bit, level);
    }

    void addPower(Database& db, DatabaseObject* object, std::string name, int level) {
      Power* p = db.powers.getPower(name);
      if (p == NULL) {
        return;
      }

      return object->addPowerByBit(p->bit, level);
    }

    void addPower(Database& db, DatabaseObject* object, std::string name) {
      return addPower(db, object, name, 0);
    }

    void addPowerByBit(DatabaseObject* object, uint32_t bit) {
      return addPowerByBit(object, bit, 0);
    }
    void addPowerByBit(DatabaseObject* object, uint32_t bit, int level) {
      return object->addPowerByBit(bit, level);
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

    bool canTeleportTo(Database& db, DatabaseObject *object, DatabaseObject* where) {
      return true;
    }
    /*
    bool controls(Database& db, DatabaseObject* object, DatabaseObject* what) {
      if (what->owner
      return true;
      }*/
  }
}
