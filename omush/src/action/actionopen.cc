/**
 * \file actionopen.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actionopen.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  ActionOpen::ActionOpen(CommandContext& context) : context_(context) {
  }

  ActionOpen& ActionOpen::from(database::DatabaseObject* object) {
    from_ = object;
    return *this;
  }

  ActionOpen& ActionOpen::to(database::DatabaseObject* object) {
    to_ = object;
    return *this;
  }

  ActionOpen& ActionOpen::name(std::string value) {
    name_ = value;
    return *this;
  }

  void ActionOpen::enact() {

    if (name_.length() == 0) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "That's a silly name for an exit.");
      return;
    }

    if (from_ == NULL) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "I don't know where you want to open the exit from. This should really never happen, so maybe you should tell someone.");
      return;
    }

    std::vector<std::string> split = splitStringIntoSegments(name_, ";", 2);

    database::DatabaseObject *e1=database::DatabaseObjectFactory::createExit(context_.db);
    e1->setProperty("name", split[0]);

    // Set alias'
    if (split.size() > 1) {
      e1->setAttribute("alias", split[1]);
    }

    context_.db->moveObject(e1, from_);;

    Notifier(*(context_.game), *(context_.db)).
      notify(context_.cmdScope.executor,
             "Opened exit " + nameFormatter(*(context_.db), context_.cmdScope.executor).format(e1) + ".");

    if (to_ != NULL) {
      e1->home(to_->dbref());
      Notifier(*(context_.game), *(context_.db)).
        notify(context_.cmdScope.executor,
               "Linked exit " + nameFormatter(*(context_.db), context_.cmdScope.executor).format(e1) + " to " + nameFormatter(*(context_.db),context_.cmdScope.executor).format(to_) + ".");
    }

  }
}  // namespace omush
