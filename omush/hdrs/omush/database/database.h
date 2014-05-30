/**
 * \file database.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_

#include <boost/algorithm/string.hpp>
#include <map>
#include "omush/database/definitions.h"

namespace omush {
  namespace database {

    /**
     * Map of Dbrefs to DatabaseObject*.
     */
    typedef std::map<Dbref, DatabaseObject*> DbMap;

    /**
     * Nested map of DbObjectType to DbMap. For keeping lists by object type.
     */
    typedef std::map<DbObjectType, DbMap> DbTypeMap;

    /**
     * Collection of DatabaseObjects.
     *
     * Contains methods for keeping track of DatabaseObjects and operating on
     * them.
     */
    class Database {
      friend class PlayerUtilities;
     public:

      /**
       * Default constructor. Sets the next Dbref to 0.
       */
      Database() : top_(0) {}

      /**
       * Add a DatabaseObject to the collection of objects by it's ref().
       *
       * Side effect: Increment the next dbref counter if needed, or remove
       * the dbref of the object from this collections list of open dbrefs.
       *
       * @param DatabaseObject* reference to a DatabaseObject constructed from
       *        DatabaseObjectFactory.
       *
       * @return true - The object was inserted correctly.
       * @return false - The object wasn't inserted due to an error.
       */
      bool addObject(DatabaseObject* obj);

      /**
       * Return if the Dbref is used by this Database or not.
       *
       * @return true - The Dbref is being used by the Database.
       * @return false - The Dbref is NOT being used by the Database.
       */
      bool hasObjectByRef(Dbref ref);

      /**
       * Return the next free dbref.
       *
       * This will not increment the reference counter, only return the next
       * dbref available. The next dbref is the lowest numerical value dbref
       * that isn't being used by the database.
       *
       * @return dbref - The next free dbref.
       */
      Dbref getNextRef();

      /**
       * Move one obbject's location to another object.
       *
       * The previous location's contents and the new location's contents will
       * both be updated.
       *
       */
      void moveObject(DatabaseObject* obj, DatabaseObject *location);
      DatabaseObject* findObjectByDbref(Dbref ref);
     private:
      DbMap allObjects_;
      DbTypeMap typedObjects_;
      Dbref top_;
    };

    class DatabaseObjectFactory {
     public:
      static DatabaseObject* createPlayer(Database* db);
      static DatabaseObject* createRoom(Database* db);
    };

  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_DATABASE_H_
