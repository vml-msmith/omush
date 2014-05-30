/**
 * \file database.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ACTION_H_
#define OMUSH_HDRS_OMUSH_ACTION_H_

#include "omush/database.h"
#include "omush/utility.h"
#include <boost/algorithm/string.hpp>

namespace omush {
  class NameFormatter {
  public:
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
      return ColorString::color(object->getProperty("name"), color);
    }
  };

  class ActionContext {
  };

  class Action {
  public:
    virtual void enact() = 0;
  protected:
    database::Database *db_;
    database::DatabaseObject* object_;
  };

  class ActionLook : public Action {
  public:
    ActionLook(database::Database *db, database::DatabaseObject *object) {
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
      response += NameFormatter().format(what_);

      std::string desc = what_->getProperty("description");
      if (desc.length() > 0) {
        response += "\n";
        response += desc;
      }

      std::vector<database::dbref> contents = what_->contents();
      std::string contentString = "";
      for (std::vector<database::dbref>::iterator iter = contents.begin();
           iter != contents.end();
           ++iter) {
        if (*iter != object_->ref()) {
          contentString += "\n";
          contentString += NameFormatter().format(db_->findObjectByDbref(*iter));
        }
      }
      if (contentString.length() > 0) {
        response += "\nContents:" + contentString;
      }

      std::cout << response << std::endl;
    }
  protected:
    database::DatabaseObject *what_;
  };

  class ActionConnect : public Action {
  public:
    ActionConnect(database::Database *db, database::DatabaseObject *object) {
      db_ = db;
      object_ = object;
    }

    void enact() {
      ActionLook(db_, object_).enact();
    }
  };
}

#endif
