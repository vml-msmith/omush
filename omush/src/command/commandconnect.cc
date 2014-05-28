/**
 * \file commandconnect.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandconnect.h"
#include <boost/algorithm/string.hpp>

namespace omush {
  CommandConnect::CommandConnect() {
    name_ = "CONNECT";
    absolute_ = false;
  }

  bool CommandConnect::run(std::string calledAs, std::string input, CommandContext context) {
    Command::run_(calledAs, input, context);

    std::vector<std::string> inputParts;
    boost::split(inputParts, input, boost::is_any_of(" "));

    if (inputParts.size() == 3) {
      context.client->isConnected = true;
      context.client->name = inputParts[1];
      context.game->sendNetworkMessage(context.descriptor, "** Connected **");
      return true;
    }

    context.game->sendNetworkMessage(context.descriptor, "I can't find a player with the username and password.");
    return true;
  }

}  // namespace omush
