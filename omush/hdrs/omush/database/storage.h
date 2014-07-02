/**
 *
 *
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_STORAGE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_STORAGE_H_

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <iostream>
#include "omush/database/databaseobject.h"
#include <boost/foreach.hpp>

namespace omush {
  namespace database {
    static int rNodeCallback(void *data,
                             int argc,
                             char **argv,
                             char **azColName);

    struct SQLRecord {
      std::map<std::string, std::string> values;
    };

    struct SQLResults {
      std::vector<SQLRecord> rows;
    };

    struct RNode;

    struct RNode {
      SQLRecord record;
      std::map<std::string, std::string> values;
      RNode* next;
    };

    class Storage {
     public:
      Storage();

      bool openFile(std::string fileName);
      bool closeFile();

      void createTables();

      SQLResults statement(std::string str);

      void insertOrUpdate(std::string table,
                          std::map<std::string, std::string> values,
                          std::vector<std::string> unique);

     protected:
      std::string fileName_;
      bool isOpen_;
      sqlite3 *dbFile_;
    };

    class ObjectPersister {
     public:
      ObjectPersister(Database* db, DatabaseObject* object);

      void write(Storage& s);
      //      ObjectPersister()
     private:
      void writeAttribute(Storage &s, std::string name, std::string value);
      void writeAttributeMap(Storage& s, AttributeMap map);
      void writeAttributes(Storage& s);
      void writeObject(Storage& s);
      void writeProperty(Storage &s, std::string name, std::string value);
      void writeProperty(Storage& s,
                         std::string name,
                         std::vector<std::string> value);
      void writePropertyMap(Storage& s, PropertyMap map);
      void writeProperties(Storage& s);

      DatabaseObject *object_;
      Database* database_;
    };
  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_STORAGE_H_
