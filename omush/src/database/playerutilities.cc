#include "omush/database/playerutilities.h"
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include <boost/algorithm/string.hpp>

namespace omush {
  namespace database {

    DatabaseObject* PlayerUtilities::findPlayerByName(Database *db, std::string name) {
      for (DbMap::iterator iter = db->typedObjects_[DbObjectTypePlayer].begin();
           iter != db->typedObjects_[DbObjectTypePlayer].end();
           ++iter) {
        if (boost::iequals(iter->second->getProperty("name"), name)) {
          return iter->second;
        }
      }
      return NULL;
    }
  }
}
