/**
 * \file command.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_

#include <boost/uuid/uuid.hpp>
#include <string>
#include "omush/game.h"

namespace omush {
  class Game;
  class Client;
  class Notifier;
  namespace database {
    class Database;
  }

  namespace network {
    typedef boost::uuids::uuid ConnectionId;
  }

  struct CommandContext {
    Game *game;
    Client *client;
    database::Database *db;
    database::Dbref dbref;
    network::ConnectionId descriptor;
  };

  class Command {
  public:
    Command() {}
    ~Command() {}
    inline std::string name() { return name_; }
    inline std::vector<std::string> shortCodes() { return shortCode_; }
    virtual bool run(std::string calledAs,
                     std::string input,
                     CommandContext context) = 0;
  protected:
    void run_(std::string calledAs,
              std::string input,
              CommandContext context);
    std::string name_;
    std::vector<std::string> shortCode_;
    bool absolute_;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
