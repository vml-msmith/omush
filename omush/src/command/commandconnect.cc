/**
 * \file commandconnect.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandconnect.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/database/playerutilities.h"
#include "omush/action/actionconnect.h"
#include "omush/game.h"

namespace omush {
  CommandConnect::CommandConnect() : ICommand("CONNECT") {
  }

  bool CommandConnect::run(CommandContext& context) {
    std::vector<std::string> inputParts;
    boost::split(inputParts, context.cmdScope.currentString, boost::is_any_of(" "));

    if (inputParts.size() == 3) {
      database::DatabaseObject *obj = database::PlayerUtilities::findPlayerByName(context.db, inputParts[1]);
      if (obj != NULL) {
        std::cout << obj->dbref() << std::endl;
        context.client->isConnected = true;
        context.game->sendNetworkMessage(context.descriptor, "** Connected **");
        context.client->dbref = obj->dbref();
        ActionConnect(context.db, context.game, obj).enact();
        return true;
      }
    }

    context.game->sendNetworkMessage(context.descriptor, "I can't find a player with the username and password.");
    return true;
  }

}  // namespace omush
