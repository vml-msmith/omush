#include "omush/database/storage.h"
#include "omush/database/helpers.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace omush {
  namespace database {
    static int rNodeCallback(void *data, int argc, char **argv, char **azColName) {
      int i;
      //std::cerr << "Callback" << std::endl;
      RNode *d = (RNode*)data;
      RNode *n = NULL;
      if (data == NULL) {
        data = new RNode();
        n = d;
        n->next = NULL;
      }
      else {
        n = d;
        while (n->next != NULL) {
          n = n->next;
        }

        n->next = new RNode();
        n = n->next;
        n->next = NULL;
      }

      //   fprintf(stderr, "%s: ", (const char*)data);
      //      std::cerr << "Callback" << std::endl;
      for(i=0; i<argc; i++){
        n->record.values.insert(std::pair<std::string,std::string>(std::string(azColName[i]), std::string(argv[i])));
        //        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      //      printf("\n");
      return 0;
    }

    Storage::Storage() {
      isOpen_ = false;
    }

    bool Storage::openFile(std::string fileName) {
      if (isOpen_) {
        // TODO(msmith) Log this. Need to close before we can open.
        return false;
      }

      fileName_ = fileName;
      int error;
      error = sqlite3_open(fileName.c_str(), &dbFile_);
      if (error) {
        // TODO(msmith) Log this. sqlite3_errmsg(db). May even create a method for it.
        return false;
      }
      else {
        isOpen_ = true;
        return true;
      }
    }

    bool Storage::closeFile() {
      if (isOpen_) {
        sqlite3_close(dbFile_);
      }

      return true;
    }

    void Storage::createTables() {
      if (!isOpen_)
        return;

      std::vector<std::string> sqls;

      sqls.push_back("CREATE TABLE OBJECTS("    \
                     "ID INT PRIMARY KEY     NOT NULL," \
                     "NAME           TEXT    NOT NULL," \
                     "TYPE           TEXT    NOT NULL);");

      sqls.push_back("CREATE TABLE ATTRIBUTES(" \
                     "OBJECTID       INT     NOT NULL," \
                     "NAME           TEXT    NOT NULL," \
                     "VALUE           TEXT    NOT NULL,"  \
                     "FOREIGN KEY(OBJECTID) REFERENCES OBJECTS(ID));");

      sqls.push_back("CREATE TABLE PROPERTIES(" \
                     "OBJECTID       INT     NOT NULL," \
                     "NAME           TEXT    NOT NULL," \
                     "VALUE           TEXT    NOT NULL,"  \
                     "FOREIGN KEY(OBJECTID) REFERENCES OBJECTS(ID));");


      for (int i = 0; i < sqls.size(); ++i) {
        statement(sqls[i]);
      }
    }


    void Storage::insertOrUpdate(std::string table,
                        std::map<std::string, std::string> values,
                        std::vector<std::string> unique) {

      std::string sql = "SELECT * FROM " + table + " WHERE ";
      std::string conditions = "1 ";

      BOOST_FOREACH(std::string key, unique) {
        conditions += " AND " + key + " = '" + values[key] + "' ";
      }
      sql += conditions + ";";

      SQLResults results = statement(sql);
      if (results.rows.size() < 1) {
        // Insert.
        sql = "INSERT INTO " + table + "(";
        std::string val = "(";
        bool first = true;
        typedef std::map<std::string, std::string> map_type;
        BOOST_FOREACH(const map_type::value_type& value, values) {
          if (!first) {
            sql += ",";
            val += ",";
          }
          sql += value.first;
          val += "'" + value.second + "'";
          first = false;
        }
        sql += ") VALUES " + val + ")";


        //        std::cout << sql << std::endl;
        statement(sql);
      }
      else {
        sql = "UPDATE " + table + " SET " ;

        bool first = true;
        typedef std::map<std::string, std::string> map_type;
        BOOST_FOREACH(const map_type::value_type& value, values) {
          if (!first) {
            sql += " , ";
          }
          sql += value.first + " = '" + value.second + "'";
          first = false;
        }
        sql += " WHERE " + conditions;;


        //        std::cout << sql << std::endl;
        statement(sql);
      }
    }

    SQLResults Storage::statement(std::string str) {
      SQLResults results;

      if (!isOpen_)
        return results;

      RNode *data = NULL;

      data = new RNode();
      data->next = NULL;
      //      data->id = "First";
      data->record.values.insert(std::pair<std::string,std::string>("test", "test"));

      char *zErrMsg = 0;
      int  rc;
      /* Execute SQL statement */

      rc = sqlite3_exec(dbFile_, str.c_str(), rNodeCallback, (void*)data, &zErrMsg);
      if (rc != SQLITE_OK ){
        std::cout << zErrMsg << std::endl;
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
      else {
        std::cout << "Executed: " << str << std::endl;
        RNode *result = data;
        result = result->next;
        while (result != NULL) {
          results.rows.push_back(result->record);
          for (std::map<std::string,std::string>::iterator iter = result->record.values.begin(); iter != result->record.values.end(); ++iter) {
             std::cout << "COUT " <<  iter->first << " = " << iter->second << std::endl;
          }
          result = result->next;
        }


        //          fprintf(stdout, "Table created successfully\n");
      }
      while (data != NULL) {
        RNode* current = data;
        data = data->next;
        delete current;
      }
      return results;
    }


    ObjectPersister::ObjectPersister(Database* db, DatabaseObject* object) {
      object_ = object;
      database_ = db;
    }

    void ObjectPersister::write(Storage& s) {
      writeObject(s);
      writeAttributes(s);
      writeProperties(s);
    }

    void ObjectPersister::writeAttribute(Storage &s, std::string name, std::string value) {
      std::map<std::string,std::string> values;
      values.insert(std::pair<std::string,std::string>("NAME", name));
      values.insert(std::pair<std::string,std::string>("OBJECTID", boost::lexical_cast<std::string>(object_->dbref())));
      values.insert(std::pair<std::string,std::string>("VALUE", value));

      std::vector<std::string> unique;
      unique.push_back("NAME");
      unique.push_back("OBJECTID");
      s.insertOrUpdate("ATTRIBUTES", values, unique);
    }

    void ObjectPersister::writeAttributeMap(Storage& s, AttributeMap map) {
      for (AttributeMap::iterator i = map.begin();
           i != map.end();
           ++i) {
        writeAttribute(s, i->first, i->second.value);
        writeAttributeMap(s, i->second.tree);
      }

    }


    void ObjectPersister::writeProperty(Storage& s, std::string name, std::string value) {
      std::map<std::string,std::string> values;
      values.insert(std::pair<std::string,std::string>("NAME", name));
      values.insert(std::pair<std::string,std::string>("OBJECTID", boost::lexical_cast<std::string>(object_->dbref())));
      values.insert(std::pair<std::string,std::string>("VALUE", value));

      std::vector<std::string> unique;
      unique.push_back("NAME");
      unique.push_back("OBJECTID");
      s.insertOrUpdate("PROPERTIES", values, unique);
    }

    void ObjectPersister::writePropertyMap(Storage& s, PropertyMap map) {
      for (PropertyMap::iterator i = map.begin();
           i != map.end();
           ++i) {
        writeProperty(s, i->first, i->second);
      }
    }

    void ObjectPersister::writeProperty(Storage& s,
                                        std::string name,
                                        std::vector<std::string> value) {
      std::stringstream ss;
      boost::archive::text_oarchive ar(ss);

      // Save the data
      ar & value;
      std::cout << ss.str().data() << std::endl;
      writeProperty(s, name, std::string(ss.str().data()));
    }

    void ObjectPersister::writeProperties(Storage& s) {
      PropertyMap props = object_->properties();
      writePropertyMap(s, props);
      writeProperty(s, "home", boost::lexical_cast<std::string>(object_->home()));
      writeProperty(s, "location", boost::lexical_cast<std::string>(object_->location()));
      writeProperty(s, "owner", boost::lexical_cast<std::string>(object_->owner()));

      writeProperty(s, "powers", getPowerList(*(database_),object_));
      writeProperty(s, "flags", getFlagList(*(database_),object_));
    }

    void ObjectPersister::writeAttributes(Storage& s) {
      AttributeMap attrs = object_->attributes();
      writeAttributeMap(s, attrs);
    }


    void ObjectPersister::writeObject(Storage& s) {
      std::map<std::string,std::string> values;
      values.insert(std::pair<std::string,std::string>("NAME", object_->getProperty("name")));
      values.insert(std::pair<std::string,std::string>("ID", boost::lexical_cast<std::string>(object_->dbref())));
      std::string type = objectTypeString(object_);
      boost::to_upper(type);
      values.insert(std::pair<std::string,std::string>("TYPE", type));

      std::vector<std::string> unique;
      unique.push_back("NAME");
      unique.push_back("ID");
      s.insertOrUpdate("OBJECTS", values, unique);
    }

  }
}
