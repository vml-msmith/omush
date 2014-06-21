/**
 * \file action.h
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
  class Action {
   public:
    virtual void enact() = 0;
   protected:

    database::Database *db_;
    database::DatabaseObject* object_;
    Game* game_;
  };
}

#endif
