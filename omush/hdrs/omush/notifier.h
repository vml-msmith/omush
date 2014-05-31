/**
 * \file notifier.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NOTIFIER_H_
#define OMUSH_HDRS_OMUSH_NOTIFIER_H_

#include <string>
#include <vector>
#include <map>
#include <boost/function.hpp>
#include "omush/database/definitions.h"

namespace omush {
  typedef std::map<std::string, std::string> StringDictionary;
  typedef boost::function<std::string(database::DatabaseObject* listener,
                                      StringDictionary dictionary)> notifyCallback;

  class Game;
  namespace database {
    typedef long Dbref;
    class DatabaseObject;
  }  // namespace database

  class Notifier {
  public:
    Notifier(Game& game, database::Database& database);
    void notify(database::DatabaseObject *object, std::string str);
//  Notifier& exclude(database::DatabaseObject *objcet);
    void notifySurroundings(database::DatabaseObject *object,
                            notifyCallback callback);
  protected:
    void lowNotify(database::DatabaseObject *object, std::string str);
//    std::vector<database::Dbref> exclude_;
    Game& game_;
    database::Database& database_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NOTIFIER_H_
