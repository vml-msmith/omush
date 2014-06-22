/**
 * \file commandcreateplayer.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandcreateplayer.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/database/playerutilities.h"
#include "omush/action/actionconnect.h"
#include "omush/game.h"

namespace omush {
  CommandCreatePlayer::CommandCreatePlayer() : ICommand("CREATE") {
  }

  CommandInfo CommandCreatePlayer::process(CommandContext& context) {
    CommandInfo info;
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (cmdSplit.size() < 2) {
      info.errorString = "There is no player with that name.";
      return info;
    }
    info.rawArgs = cmdSplit[1];
    cmdSplit = splitStringIntoSegments(cmdSplit[1], " ", 2, rightToLeft);

    std::vector<std::string> arg;
    arg.push_back(cmdSplit[0]);

    if (cmdSplit.size() > 1) {
      arg.push_back(cmdSplit[1]);
    }
    else {
      arg.push_back("");
    }

    info.eqArgs.push_back(arg);

    return info;
  }

  bool CommandCreatePlayer::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      context.game->sendNetworkMessage(context.descriptor, info.errorString);
      return true;
    }

    database::DatabaseObject *player = NULL;
    player = database::PlayerUtilities::findPlayerByName(context.db,
                                                         info.eqArgs[0][0]);

    if (player == NULL) {
      context.game->sendNetworkMessage(context.descriptor,
                                       "There is no player with that name.");
      return true;
    }

    if (!player->comparePassword(info.eqArgs[0][1])) {
      context.game->sendNetworkMessage(context.descriptor,
                                       "There is no player with that name.");
      return true;
    }

    context.client->isConnected = true;
    context.game->sendNetworkMessage(context.descriptor, "** Connected **");
    context.client->dbref = player->dbref();
    context.cmdScope.enactor = player;
    context.cmdScope.executor = player;
    context.cmdScope.caller = player;

    ActionConnect(context).enact();

    return true;
  }

}  // namespace omush
