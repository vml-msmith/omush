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

  namespace network {
    typedef boost::uuids::uuid ConnectionId;
  }

  struct CommandContext {
    Game *game;
    Client *client;
    network::ConnectionId descriptor;
  };

  class Command {
  public:
    Command() {}
    ~Command() {}
    inline std::string name() { return name_; }
    virtual bool run(std::string calledAs,
                     std::string input,
                     CommandContext context) = 0;
  protected:
    void run_(std::string calledAs,
              std::string input,
              CommandContext context);
    std::string name_;
    bool absolute_;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
