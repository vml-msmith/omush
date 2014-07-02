/**
 * \file actiondivisionattach.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/action/actiondivisionattach.h"
#include "omush/database/database.h"
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include "omush/database/utilityfactories.h"
#include <boost/foreach.hpp>

namespace omush {
  ActionDivisionAttach::ActionDivisionAttach(CommandContext& context)
    : context_(context), target_(NULL), division_(NULL) {
  }

  ActionDivisionAttach& ActionDivisionAttach::target(database::DatabaseObject* target) {
    target_ = target;
    return *this;
  }

  ActionDivisionAttach& ActionDivisionAttach::division(database::DatabaseObject* division) {
    division_ = division;
    return *this;
  }


  bool ActionDivisionAttach::hasPermissionTarget() {
    //
    return true;
  }

  bool ActionDivisionAttach::hasPermissionDivision() {
    //
    return true;
  }

  void ActionDivisionAttach::enact() {
    if (target_ == NULL) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "I don't know what that target is.");
      return;
    }

    if (division_ == NULL) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "I don't know what that target is.");
      return;
    }

    if (!hasPermissionTarget()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "You don't have permission to attach that.");
      return;
    }

    if (!hasPermissionDivision()) {
      Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                        "You don't have permission to attach to that division.");
      return;
    }


    // If target.type == div
    //   Is the div in the div history?
    std::vector<database::DatabaseObject*> divs = divisionAncestors(*(context_.db), division_);
    BOOST_FOREACH(database::DatabaseObject* target, divs) {
      if (target == target_) {
        Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                          "A division cannot be an ancestor of itself.");
        return;
      }
    }

    //    database::DatabaseObject *targetEmpire

    database::NameFormatter nf = nameFormatter(*(context_.db), object_);

    target_->setAttribute("DIVISION", nf.formatDbref(division_->dbref()));
    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
                                                      "You have attached " +
                                                      nf.format(target_) +
                                                      " to " + nf.format(division_) + ".");
    Notifier(*(context_.game), *(context_.db)).notify(target_,
                                                      "You have been attached to " +
                                                      nf.format(division_) +
                                                      " by " + nf.format(target_) + ".");

    /*
    database::DatabaseObject *r1 = database::DatabaseObjectFactory::createThing(context_.db);
    r1->setProperty("name", name_);

    context_.db->changeOwnership(r1, context_.cmdScope.executor);
    context_.db->moveObject(r1, context_.cmdScope.executor);
    */
    //    Notifier(*(context_.game), *(context_.db)).notify(context_.cmdScope.executor,
    //                            "Created " + NameFormatter(object_).format(r1) + ".");

  }
}  // namespace omush
