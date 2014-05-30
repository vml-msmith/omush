/**
 * \file database.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_H_

#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>

namespace omush {
  namespace database {

    enum DbObjectType {
      DbObjectTypePlayer,
      DbObjectTypeThing,
      DbObjectTypeRoom,
    };

    typedef long dbref;

    struct DatabaseAttribute;

    struct DatabaseAttribute {
      std::map<std::string,DatabaseAttribute> tree;
      std::string value;
    };

    class DatabaseObjectFactory;
    class DatabaseObject {
      friend class DatabaseObjectFactory;
      friend class Database;
     public:
      dbref ref();
std::string getProperty(std::string propertyName);
DatabaseAttribute getAttribute(std::string name);
void setProperty(std::string name, std::string value);
void setAttribute(std::string name, std::string value);
DbObjectType type();
dbref location();
std::vector<dbref> contents();

     protected:
      typedef std::map<std::string,std::string> PropertyMap;
      typedef std::map<std::string,DatabaseAttribute> AttributeMap;
      dbref ref_;
      PropertyMap properties_;
      AttributeMap attributes_;
      DbObjectType type_;
      dbref location_;
      std::vector<dbref> contents_;
    };

typedef std::map<dbref,DatabaseObject*> dbMap;
typedef std::map<DbObjectType,dbMap> dbTypeMap;

    class PlayerUtilities;
    class Database {
      friend class PlayerUtilities;
    public:
      Database() : top_(0) {}
      void addObject(DatabaseObject* obj);
      dbref getNextRef();
      void moveObject(DatabaseObject* obj, dbref ref);
DatabaseObject* findObjectByDbref(dbref ref);
    private:
      dbMap allObjects_;
      dbTypeMap typedObjects_;
      dbref top_;
    };

    class DatabaseObjectFactory {
    public:
      static DatabaseObject* createPlayer(Database* db) {
        DatabaseObject* obj = new DatabaseObject();
        obj->type_ = DbObjectTypePlayer;
        obj->setProperty("name", "Guest");
        obj->ref_ = db->getNextRef();
        obj->location_ = obj->ref_;
        db->addObject(obj);

        return obj;
      }
      static DatabaseObject* createRoom(Database* db) {
        DatabaseObject* obj = new DatabaseObject();
        obj->type_ = DbObjectTypeRoom;
        obj->setProperty("name", "room");
        obj->ref_ = db->getNextRef();
        obj->location_ = obj->ref_;
        db->addObject(obj);

        return obj;
      }
    };

    class PlayerUtilities {
    public:
      static DatabaseObject* findPlayerByName(Database *db, std::string name) {
        for (dbMap::iterator iter = db->typedObjects_[DbObjectTypePlayer].begin();
             iter != db->typedObjects_[DbObjectTypePlayer].end();
             ++iter) {
          if (boost::iequals(iter->second->getProperty("name"), name)) {
            return iter->second;
          }
        }
      }
    };
  }
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_H_
