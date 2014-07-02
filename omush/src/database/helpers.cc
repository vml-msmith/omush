/**
 *
 */
#include "omush/database/helpers.h"
#include "omush/power.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

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

    database::DatabaseObject* getObjectFromAttribute(Database& db,
                                                     DatabaseObject* object,
                                                     std::string name) {
      int dbref = parseStringDbref(object->getAttribute(name).value);
      if (dbref >= 0) {
        return dbrefToObject(db, dbref);
      }
      return NULL;
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
      case database::DbObjectTypeDivision:
        return "Division";
        break;
      }
      return "Unknown";
    }


    bool hasPowerOver(Database& db,
                      DatabaseObject* object,
                      DatabaseObject* target,
                      std::string name) {

      if (db.root() == object)
        return true;

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
        objectDivision = getObjectFromAttribute(db, objectOwner, "division");

        int objectEmpireDbref = parseStringDbref(objectOwner->getAttribute("empire").value);
        if (objectEmpireDbref >= 0)
          objectEmpire = dbrefToObject(db, objectEmpireDbref);

        try {
          objectClass = boost::lexical_cast<int>(objectOwner->getAttribute("classvalue").value);
        }
        catch (boost::bad_lexical_cast&) {}
      }

      bool inDivision = objectInDivisionTree(db, targetOwner, objectDivision);

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
        if (inDivision && objectDivision != NULL)
          return true;
        return false;
      }

      if (powers[5]) {
        if (objectClass > targetClass && inDivision && objectDivision != NULL)
          return true;
        return false;
      }

      // Everyone, return true.
      if (powers[0])
        return true;

      return false;
    }

    bool hasFlag(Database& db, DatabaseObject* object, std::string name) {
      Flag* f = db.flags.getFlag(name);
      if (f == NULL) {
        return false;
      }

      return object->hasFlagByBit(f->bit);
    }

    void addFlag(Database& db, DatabaseObject* object, std::string name) {
      Flag* f = db.flags.getFlag(name);
      if (f == NULL) {
        return;
      }

      return object->addFlagByBit(f->bit);
    }

    void addFlagByBit(DatabaseObject* object, uint64_t bit) {
      return object->addPowerByBit(bit);
    }

    void removeFlagByBit(DatabaseObject* object, uint64_t bit) {
      return object->removeFlagByBit(bit);
    }

    void removeFlag(Database& db, DatabaseObject* object, std::string name) {
      Flag* f = db.flags.getFlag(name);
      if (f == NULL) {
        return;
      }

      return object->removeFlagByBit(f->bit);
    }


    bool hasPower(Database& db, DatabaseObject* object, std::string name) {
      return hasPower(db, object, name, 0);
    }

    bool hasPower(Database& db, DatabaseObject* object, std::string name, int level) {
      if (db.root() == object)
        return true;

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

    std::vector<database::DatabaseObject*> divisionAncestors(database::Database& db, database::DatabaseObject *object) {
      std::vector<DatabaseObject*> divisions;
      while (object != NULL) {
        divisions.push_back(object);
        object = getObjectFromAttribute(db, object, "division");
      }
      return divisions;
    }

    database::DatabaseObject* getAncestorEmpire(database::Database& db, database::DatabaseObject *object) {
      std::vector<database::DatabaseObject*> divs = divisionAncestors(db, object);
      BOOST_FOREACH(database::DatabaseObject* target, divs) {
        if (hasFlag(db, target, "empire"))
          return target;
      }

      return NULL;
    }

    bool objectInDivisionTree(database::Database& db,
                              database::DatabaseObject* object,
                              database::DatabaseObject* division) {
      std::vector<database::DatabaseObject*> divs = divisionAncestors(db, object);
      DatabaseObject* targetDiv = getObjectFromAttribute(db, object, "division");

      BOOST_FOREACH(DatabaseObject* div, divs) {
        if (div == targetDiv)
          return true;
      }
      return false;
    }



    std::vector<std::string> getPowerList(database::Database& db,
                                         database::DatabaseObject* object) {
      std::vector<std::string> powers;
      std::vector<std::string> levels;
      levels.push_back("0");
      levels.push_back("1");
      levels.push_back("2");
      levels.push_back("3");
      levels.push_back("4");
      levels.push_back("5");

      std::map<std::string,Power> allPowers = db.powers.getAllPowers();
      for (std::map<std::string,Power>::iterator it = allPowers.begin();
           it != allPowers.end();
           ++it) {
        for (int i = 5; i >= 0; --i) {
          if (object->hasPowerByBit(it->second.bit, i)) {
            powers.push_back(levels[i] + ":" + it->first);
            continue;
          }
        }
      }

      return powers;
    }

    std::vector<std::string> getFlagsList(database::Database& db,
                                          database::DatabaseObject* object) {
      std::vector<std::string> flags;

      std::map<std::string,Flag> allFlags = db.flags.getAllFlags();
      for (std::map<std::string,Flag>::iterator it = allFlags.begin();
           it != allFlages.end();
           ++it) {
        if (object->hasFlagByBit(it->second.bit)) {
          flags.push_back(it->first);
          continue;
        }
      }

      return flags;
    }
  }
}
