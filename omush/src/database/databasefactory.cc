/**
 * \file databasefactory.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/databasefactory.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/storage.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace omush {
  namespace database {
    Database* DatabaseFactory::createDatabase() {
      Database* db = new Database();
      setupFlags(db);
      setupPowers(db);
      setupAttributes(db);

      loadObjectsFromDatabaseFile(db, "objects.db");
      if (db->getNextDbref() == 0) {
        defaultObjects(db);
      } else {
    for (int i = 0; i < db->getNextDbref(); ++i) {
      database::DatabaseObject* object = db->findObjectByDbref(i);
      if (object == NULL)
        continue;

      std::cout << "DB Object: " << object->getProperty("name") << std::endl;
    }
      }

      db->setRoot(db->findObjectByDbref(1));
      return db;
    }

    void DatabaseFactory::loadObjectsFromDatabaseFile(Database *db,
                                                      std::string file) {
      Storage storage;
      if (!storage.openFile(file))
        return;

      storage.createTables();


      std::string sql = "SELECT * FROM OBJECTS";
      SQLResults results = storage.statement(sql);

      BOOST_FOREACH(SQLRecord record, results.rows) {
        database::DatabaseObject *object = NULL;
        std::cout << "Have record: " << record.values["ID"] << std::endl;
        try {
          Dbref dbref = boost::lexical_cast<int>(record.values["ID"]);
          std::cout << "CREATE OBJECT" << std::endl;
          object = DatabaseObjectFactory::createObject(db,
                                                       record.values["TYPE"],
                                                       dbref);
          std::cout << "Created: " << object->getProperty("name") << std::endl;
        } catch (const boost::bad_lexical_cast &) {
          continue;
        }
      }

      sql = "SELECT * FROM PROPERTIES";
      results = storage.statement(sql);
      BOOST_FOREACH(SQLRecord record,  results.rows) {
        Dbref dbref;
        std::cout << "Have record: " << record.values["OBJECTID"] << std::endl;
        try {

          dbref = boost::lexical_cast<int>(record.values["OBJECTID"]);
          std::cout << "HERE: " << dbref << std::endl;
        } catch (const boost::bad_lexical_cast &) {
          continue;
        }

        database::DatabaseObject *object = NULL;
        object = db->findObjectByDbref(dbref);
        if (object == NULL)
          continue;

        std::cout << "Good Object" << std::endl;

        if (boost::iequals(record.values["NAME"], "LOCATION")) {
          try {
            dbref = boost::lexical_cast<int>(record.values["VALUE"]);
            database::DatabaseObject *loc = NULL;
            loc = db->findObjectByDbref(dbref);
            if (loc != NULL)
              db->moveObject(object, loc);
          } catch (const boost::bad_lexical_cast &) {
          }
        }
        else if (boost::iequals(record.values["NAME"], "HOME")) {
          try {
            dbref = boost::lexical_cast<int>(record.values["VALUE"]);
            object->home(dbref);
          } catch (const boost::bad_lexical_cast &) {
          }
        }
        else if (boost::iequals(record.values["NAME"], "POWERS")) {
          std::string v  = record.values["VALUE"];

          std::stringstream ss;
          ss << v.c_str();

          boost::archive::text_iarchive ia(ss);
          std::vector<std::string> list;
          ia >> list;
          BOOST_FOREACH(std::string str, list) {
            std::string levelStr = str.substr(0,1);
            std::string powerStr = str.substr(2,str.length());
            uint64_t f = db->powers.getPowerBit(powerStr);
            if (f != 0) {
              try {
                int level = boost::lexical_cast<int>(levelStr);
                object->addPowerByBit(f, level);
              } catch (const boost::bad_lexical_cast &) {}
            }
          }
        }
        else if (boost::iequals(record.values["NAME"], "OWNER")) {
          try {
            dbref = boost::lexical_cast<int>(record.values["VALUE"]);
            object->home(dbref);

            database::DatabaseObject *loc = NULL;
            loc = db->findObjectByDbref(dbref);
            if (loc != NULL)
              db->changeOwnership(object, loc);
          } catch (const boost::bad_lexical_cast &) {
          }
        }
        else {
          std::cout << "Prop " << record.values["NAME"] << " " << record.values["VALUE"] << std::endl;
          object->setProperty(record.values["NAME"],
                              record.values["VALUE"]);
        }
      }

      sql = "SELECT * FROM ATTRIBUTES";
      results = storage.statement(sql);
      BOOST_FOREACH(SQLRecord record,  results.rows) {
        Dbref dbref;
        try {
          dbref = boost::lexical_cast<int>(record.values["id"]);
        } catch (const boost::bad_lexical_cast &) {
          continue;
        }

        database::DatabaseObject *object = NULL;
        object = db->findObjectByDbref(dbref);
        if (object == NULL)
          continue;
        object->setAttribute(record.values["NAME"], record.values["VALUE"]);
      }

      storage.closeFile();
    }

    void DatabaseFactory::setupFlags(Database* db) {
      db->flags.addFlag(Flag("Director", 'D'));
      db->flags.addFlag(Flag("Admin", 'A'));
      db->flags.addFlag(Flag("Hidden", 'h'));
    }

    void DatabaseFactory::setupPowers(Database* db) {
      db->powers.add(Power("Unlimited Quota"));
      db->powers.add(Power("Unlimited Credits"));
      db->powers.add(Power("Teleport Any Object"));
      db->powers.add(Power("Teleport Anywhere"));
      db->powers.add(Power("Modify Any Object"));
      db->powers.add(Power("Link Anywhere"));
      db->powers.add(Power("Hide From Everyone"));
      db->powers.add(Power("Examine Any Object"));
      db->powers.add(Power("Broadcast"));
      db->powers.add(Power("Boot"));
    }

    void DatabaseFactory::setupAttributes(Database* db) {
      db->attributes.addAttribute("empire");
      db->attributes.addAttribute("classnum");
      db->attributes.addAttribute("division");
    }

    void DatabaseFactory::defaultObjects(Database *db) {
      database::DatabaseObject *r1=database::DatabaseObjectFactory::createRoom(db);
      database::DatabaseObject *p1=database::DatabaseObjectFactory::createPlayer(db);
      database::DatabaseObject *r2=database::DatabaseObjectFactory::createRoom(db);
      database::DatabaseObject *e1=database::DatabaseObjectFactory::createExit(db);
      database::DatabaseObject *e2=database::DatabaseObjectFactory::createExit(db);

      r1->setProperty("name", "Room Zero");
      p1->setProperty("name", "One");
      r2->setProperty("name", "Master Room");
      e1->setProperty("name", "Out");
      e2->setProperty("name", "Out");

      e1->home(r2->dbref());
      e2->home(r1->dbref());
      db->moveObject(p1, r1);
      db->moveObject(e1, r1);
      db->moveObject(e2, r2);

    }
  }  // namespace database
}  // namespace omush
