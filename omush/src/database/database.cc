/// Copyright 2014 Michael Smith

#include "omush/database/database.h"
#include <string>

namespace omush {
  namespace database {
    Database::Database() : highRef_(0) {  }

    void Database::registerObject(DatabaseObject* db_object) {
      DatabaseObjectType db_type = db_object->getType();
      dbref dbref_number = db_object->getDbref();
      objectsByType_[db_type][dbref_number] = db_object;
      allObjects_[dbref_number] = db_object;

      if (dbref_number > highRef_) {
        highRef_ = dbref_number;
      }
    }

    DatabaseObject* Database::getObjectByDbref(dbref ref) {
      if (allObjects_.count(ref) == 0) {
        return NULL;
      }

      return allObjects_[ref];
    }

    Player* Player::findByNameAndPass(Database* db,
                                      std::string name,
                                      std::string pass) {
      if (db->objectsByType_.count(DBPlayer) == 0) {
        return NULL;
      }

      std::map<dbref, DatabaseObject*>::iterator i;
      for (i = db->objectsByType_[DBPlayer].begin();
           i != db->objectsByType_[DBPlayer].end();
           ++i) {
        database::Player *p = reinterpret_cast<Player*>(i->second);
        if (p->matchNameAndPass(name, pass)) {
          return p;
        }
      }
      return NULL;
    }

  }  // namespace database
}  // namespace omush
