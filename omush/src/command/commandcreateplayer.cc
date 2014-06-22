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
#include "omush/database/databaseobjectfactory.h"
#include "omush/database/helpers.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace omush {
  CommandCreatePlayer::CommandCreatePlayer() : ICommand("CREATE") {
  }

  CommandInfo CommandCreatePlayer::process(CommandContext& context) {
    CommandInfo info;
    std::vector<std::string> cmdSplit = splitStringIntoSegments(context.cmdScope.currentString, " ", 2);
    if (cmdSplit.size() < 2) {
      info.errorString = "That name is not allowed.";
      return info;
    }
    info.rawArgs = cmdSplit[1];
    cmdSplit = splitStringIntoSegments(cmdSplit[1], " ", 2, rightToLeft);

    std::vector<std::string> arg;
    arg.push_back(cmdSplit[0]);

    if (cmdSplit.size() > 1) {
      arg.push_back(cmdSplit[1]);
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

    if (player != NULL) {
      context.game->sendNetworkMessage(context.descriptor,
                                       "There is already a player with that name.");
      return true;
    }

    if (info.eqArgs[0].size() < 2) {
      context.game->sendNetworkMessage(context.descriptor,
                                       "The password is invalid (or missing).");
      return true;
    }


    // Because creation of a player can happen w/ @pcreate or with the connection
    // screen, I'm not sure if it's appropriate for an Action item. For now,
    // we're going to just do the work here. I want to revist this later though.
    //
    // TODO(msmith): revisit this.
    /*
    context.cmdScope.enactor = NULL;
    context.cmdScope.executor = NULL;
    context.cmdScope.caller = NULL;
    ActionPlayerCreate(context).enact();
    */

    //context.client->isConnected = true;
    //    context.game->sendNetworkMessage(context.descriptor, "** Connected **");
    //context.client->dbref = player->dbref();
    /*
    context.cmdScope.enactor = NULL;
    context.cmdScope.executor = NULL;
    context.cmdScope.caller = NULL;
    */
    //    ActionConnect(context).enact();
    player = database::DatabaseObjectFactory::createPlayer(context.db);
    player->setProperty("name", info.eqArgs[0][0]);
    player->setPassword(info.eqArgs[0][1]);
    context.db->moveObject(player, dbrefToObject(*context.db, 0));

    context.client->isConnected = true;
    context.game->sendNetworkMessage(context.descriptor, "** Connected **");
    context.client->dbref = player->dbref();
    context.client->connectTime = boost::posix_time::second_clock::universal_time();


    context.cmdScope.enactor = player;
    context.cmdScope.executor = player;
    context.cmdScope.caller = player;

    ActionConnect(context).enact();
    return true;
  }

}  // namespace omush
