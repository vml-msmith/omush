#include "omush/database/databaseobjectfactory.h"
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    DatabaseObject*  DatabaseObjectFactory::createPlayer(Database* db) {
        DatabaseObject* obj = new DatabaseObject();
        obj->type_ = DbObjectTypePlayer;
        obj->setProperty("name", "Guest");
        obj->dbref_ = db->getNextDbref();
        obj->location_ = obj->dbref_;
        obj->home_ = obj->dbref_;
        db->addObject(obj);

        return obj;
    }

    DatabaseObject* DatabaseObjectFactory::createRoom(Database* db) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeRoom;
      obj->setProperty("name", "room");
      obj->dbref_ = db->getNextDbref();
      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);

      return obj;
    }

    DatabaseObject* DatabaseObjectFactory::createExit(Database* db) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeExit;
      obj->setProperty("name", "room");
      obj->dbref_ = db->getNextDbref();
      obj->location_ = obj->dbref_;
      obj->home_ = obj->dbref_;
      db->addObject(obj);
      return obj;
    }
  }
}
