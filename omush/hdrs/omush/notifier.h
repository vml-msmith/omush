/**
 * \file notifier.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NOTIFIER_H_
#define OMUSH_HDRS_OMUSH_NOTIFIER_H_

#include <string>
#include <vector>

namespace omush {
  class Game;
  namespace database {
    typedef long Dbref;
    class DatabaseObject;
  }  // namespace database

  class Notifier {
  public:
    Notifier(Game& game);

    void notify(database::DatabaseObject *object, std::string str);
    Notifier& exclude(database::Dbref dbref);
  protected:
    void lowNotify(database::Dbref dbref, std::string str);
    std::vector<database::Dbref> exclude_;
    Game& game_;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NOTIFIER_H_
