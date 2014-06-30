#include "omush/database/databaseobjectfactory.h"
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    std::map<std::string,DbObjectType> DatabaseObjectFactory::types;


    DatabaseObject* DatabaseObjectFactory::createObject(Database* db,
                                                        std::string typeName) {
      return createObject(db, typeName, db->getNextDbref());
    }

    DatabaseObject* DatabaseObjectFactory::createObject(Database* db,
                                                        std::string typeName,
                                                        Dbref dbref) {
      if (types.size() == 0) {
        DatabaseObjectFactory::types["PLAYER"] = DbObjectTypePlayer;
        DatabaseObjectFactory::types["THING"] = DbObjectTypeThing;
        DatabaseObjectFactory::types["ROOM"] = DbObjectTypeRoom;
        DatabaseObjectFactory::types["EXIT"] = DbObjectTypeExit;
      }


      DbObjectType type = DbObjectTypeThing;
      boost::to_upper(typeName);
      if (types.find(typeName) != types.end())
        type = types[typeName];

      switch (type) {
      case DbObjectTypePlayer:
        return DatabaseObjectFactory::createPlayer(db, dbref);
        break;
      case DbObjectTypeExit:
        return DatabaseObjectFactory::createExit(db, dbref);
        break;
      case DbObjectTypeRoom:
        return DatabaseObjectFactory::createRoom(db, dbref);
        break;
      case DbObjectTypeThing:
      default:
        return DatabaseObjectFactory::createThing(db, dbref);
        break;
      }
    }

    DatabaseObject*  DatabaseObjectFactory::createPlayer(Database* db) {
      return createPlayer(db, db->getNextDbref());
    }

    DatabaseObject*  DatabaseObjectFactory::createPlayer(Database* db,
                                                         Dbref dbref) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypePlayer;
      obj->setProperty("name", "Guest");
      obj->dbref_ = dbref;

      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);
      db->changeOwnership(obj, obj);
      return obj;
    }

    DatabaseObject* DatabaseObjectFactory::createRoom(Database* db) {
      return createRoom(db, db->getNextDbref());
    }

    DatabaseObject* DatabaseObjectFactory::createRoom(Database* db,
                                                      Dbref dbref) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeRoom;
      obj->setProperty("name", "room");
      obj->dbref_ = dbref;

      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);
      db->changeOwnership(obj, obj);
      return obj;
    }

    DatabaseObject* DatabaseObjectFactory::createExit(Database* db) {
      return createExit(db, db->getNextDbref());
    }

    DatabaseObject* DatabaseObjectFactory::createExit(Database* db,
                                                      Dbref dbref) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeExit;
      obj->setProperty("name", "exit");
      obj->dbref_ = dbref;

      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);
      db->changeOwnership(obj, obj);
      return obj;
    }

    DatabaseObject* DatabaseObjectFactory::createThing(Database* db) {
      return createThing(db, db->getNextDbref());
    }


    DatabaseObject* DatabaseObjectFactory::createThing(Database* db,
                                                       Dbref dbref) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeThing;
      obj->setProperty("name", "thing");
      obj->dbref_ = dbref;
      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);
      db->changeOwnership(obj, obj);
      return obj;
    }
  }
}
