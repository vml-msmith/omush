/**
 * \file database.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_H_
#define OMUSH_HDRS_OMUSH_ACTION_H_

#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/utility.h"
#include <boost/algorithm/string.hpp>
#include "omush/notifier.h"
#include <vector>
#include <boost/lexical_cast.hpp>

namespace omush {
  class TargetMatcher {
    database::DatabaseObject* match(database::DatabaseObject* enactor,
                                   std::string match) {
      return NULL;
    }
    /*
    def __init__(self):
        self.type = None

    def options(self, data):
        if 'type' in data:
            if isinstance(data['type'], tuple):
                self.type = data['type']
            else:
                self.type = (data['type'])

        return self

    def match(self, enactor, target_string):

        ustring = target_string.upper()
        if ustring == 'ME':
            return enactor
        elif ustring == 'HERE':
            return enactor.location
        elif DbrefFormatter().is_Dbref(target_string) == True:
            item = global_database.get_object_by_Dbref(target_string)

            if self.type == None or item == None or item.type in self.type:
                return item

        location = enactor.location

        for item in location.contents:
            item_name = item.name.upper()

            if item_name == ustring and item.type in self.type:
                return item

        if location.type == 'Room':

            if self.type == None or 'Exit' in self.type:
                for item in location.exits:

                    item_name = item.name.upper()

                    if item_name == ustring:
                        return item

        return None
    */
  };
  class NameFormatter {
  public:
    NameFormatter(database::DatabaseObject *looker) : looker_(looker) {
    }

    std::string format(database::DatabaseObject *object) {
      std::string color = "";
      switch (object->type()) {
      case database::DbObjectTypePlayer:
        color = "red";
        break;
      case database::DbObjectTypeRoom:
      default:
        color = "green";
        break;
      }
      std::string output = object->getProperty("name");

      output += std::string("(")
        + formatDbref(object->ref())
        + std::string(")");

      return ColorString::color(output, color);
    }
    std::string formatDbref(database::Dbref ref) {
      return "#" + boost::lexical_cast<std::string>(ref);
    }
  private:
    database::DatabaseObject *looker_;

  };

  class ActionContext {
  };

  class Action {
  public:
    virtual void enact() = 0;
  protected:
    database::Database *db_;
    database::DatabaseObject* object_;
    Game* game_;
  };

  class ActionLook : public Action {
  public:
    ActionLook(database::Database *db, Game *game, database::DatabaseObject *object) {
      game_ = game;
      db_ = db;
      object_ = object;
      what_ = NULL;
    }

    void enact() {
      std::cout << "Ty look" <<std::endl;
      if (what_ == NULL) {
        what_ = db_->findObjectByDbref(object_->location());
      }
      if (what_ == NULL) {
        std::cout << "Nill" << std::endl;
        // Notify can't see the object.
        return;
      }
      std::string response = "";
      response += NameFormatter(object_).format(what_);

      std::string desc = what_->getProperty("description");
      if (desc.length() > 0) {
        response += "\n";
        response += desc;
      }

      std::vector<database::Dbref> contents = what_->contents();
      std::string contentString = "";
      for (std::vector<database::Dbref>::iterator iter = contents.begin();
           iter != contents.end();
           ++iter) {
        if (*iter != object_->ref()) {
          contentString += "\n";
          contentString += NameFormatter(object_).format(db_->findObjectByDbref(*iter));
        }
      }
      if (contentString.length() > 0) {
        response += "\nContents:" + contentString;
      }

      Notifier(*game_).notify(object_, response);
    }
  protected:
    database::DatabaseObject *what_;
  };

  class ActionConnect : public Action {
  public:
    ActionConnect(database::Database *db,
                  Game *game,
                  database::DatabaseObject *object) {
      db_ = db;
      object_ = object;
      game_ = game;
    }

    void enact() {
      ActionLook(db_, game_, object_).enact();
    }
  };


  class ActionHuh : public Action {
  public:
    ActionHuh(database::Database *db,
              Game *game,
              database::DatabaseObject *object) {
      db_ = db;
      object_ = object;
      game_ = game;
    }

    void enact() {
      Notifier(*game_).notify(object_, "Huh? I don't know that command.");
    }
  };

  class ActionSay : public Action {
  public:
    ActionSay(database::Database *db,
              Game *game,
              database::DatabaseObject *object) {
      db_ = db;
      object_ = object;
      game_ = game;
      what_ = "";
    }

    ActionSay& what(std::string str) {
      what_ = str;
      return *this;
    }

    void enact() {
      //      Notifier(*game_).exclude().notify(object_->ref(), "You say, \"" + what_ + "\"");
    }
  protected:
    std::string  what_;
  };
}

#endif
