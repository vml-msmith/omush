#include "omush/database/database.h"
#include "omush/database/databaseobject.h"

namespace omush {
  namespace database {
    DatabaseObject*  DatabaseObjectFactory::createPlayer(Database* db) {
        DatabaseObject* obj = new DatabaseObject();
        obj->type_ = DbObjectTypePlayer;
        obj->setProperty("name", "Guest");
        obj->ref_ = db->getNextRef();
        obj->location_ = obj->ref_;
        db->addObject(obj);

        return obj;
    }
    DatabaseObject* DatabaseObjectFactory::createRoom(Database* db) {
      DatabaseObject* obj = new DatabaseObject();
      obj->type_ = DbObjectTypeRoom;
      obj->setProperty("name", "room");
      obj->ref_ = db->getNextRef();
      obj->location_ = obj->ref_;
      db->addObject(obj);

      return obj;
    }
  }
}
