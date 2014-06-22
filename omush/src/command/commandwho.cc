/**
 * \file commandwho.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandwho.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "omush/database/database.h"
#include "omush/database/playerutilities.h"
#include "omush/action/actionconnect.h"
#include "omush/game.h"
#include "omush/database/databaseobject.h"
#include "omush/database/helpers.h"

namespace omush {
  CommandWho::CommandWho() : ICommand("WHO") {
  }

  CommandInfo CommandWho::process(CommandContext& context) {
    CommandInfo info;
    return info;
  }

  std::string CommandWho::formatOnFor(boost::posix_time::time_duration time) {
    int t = time.ticks() / 1000000;

    int hours = t / 3600;
    int minutes = (t - (hours * 3600)) / 60;

    std::string sHours = boost::lexical_cast<std::string>(hours);
    if (sHours.length() <= 1)
      sHours = "0" + sHours;

    std::string sMinutes = boost::lexical_cast<std::string>(minutes);
    if (sMinutes.length() <= 1)
      sMinutes= "0" + sMinutes;

    return sHours + ":" + sMinutes;
  }

  std::string CommandWho::formatIdle(boost::posix_time::time_duration time) {
    int t = time.ticks() / 1000000;
    std::cout << "HHHHHHHH" << std::endl;
    int hours = t/ 3600;
    if (hours > 0) {
      std::string sHours = boost::lexical_cast<std::string>(hours);
      sHours += "h";
      return sHours;
    }
    std::cout << "HHHHHHHH" << std::endl;
    int minutes = t / 60;
    if (minutes > 0) {
      std::string sHours = boost::lexical_cast<std::string>(minutes);
      sHours += "m";
      return sHours;
    }

    int secs = t;
    return boost::lexical_cast<std::string>(t) + "s";
  }

  std::string CommandWho::formatWhoColumn(std::vector<WhoColumn> columns) {
    std::string line = "";

    for (std::vector<WhoColumn>::iterator it = columns.begin();
         it != columns.end();
         ++it) {
      std::string val = (*it).value;
      size_t size = (*it).length;

      if (val.length() > size - 1) {
        val = val.substr(0, size - 1);
      }

      line += val;
      for (int i = 0; i < size - val.length(); ++i) {
        line += " ";
      }
    }

    return line;
  }

  bool CommandWho::canAddHidden(CommandContext& context,
                                database::DatabaseObject* player) {
    return true;
  }

  bool CommandWho::visible(CommandContext& context,
                           database::DatabaseObject* player) {
    return true;
  }

  bool CommandWho::run(CommandContext& context) {
    CommandInfo info = process(context);
    if (info.errorString.length() > 0) {
      context.game->sendNetworkMessage(context.descriptor, info.errorString);
      return true;
    }


    size_t nameSize = 15;
    size_t flagSize = 8;
    size_t onForSize = 8;
    size_t idleSize = 8;
    size_t classSize = 20;
    size_t empireSize = 6;
    size_t doingSize = 15;

    std::vector<WhoColumn> columns;
    columns.push_back(WhoColumn("Name", nameSize));
    columns.push_back(WhoColumn("Flags", flagSize));
    columns.push_back(WhoColumn("OnFor", onForSize));
    columns.push_back(WhoColumn("Idle", idleSize));
    columns.push_back(WhoColumn("Class/Pos", classSize));
    columns.push_back(WhoColumn("Emp", empireSize));
    columns.push_back(WhoColumn("Doing", doingSize));

    context.game->sendNetworkMessage(context.descriptor, formatWhoColumn(columns));

    boost::posix_time::ptime currentTime = boost::posix_time::second_clock::universal_time();

    ClientMap& clients = context.game->clientList();

    int playerCount = 0;
    int hiddenCount = 0;

    for (ClientMap::reverse_iterator it = clients.rbegin();
         it != clients.rend();
         ++it) {
      Client* c = &(it->second);

      if (!c->isConnected) {
        std::cout << "Player is not connected" << std::endl;
        continue;
      }

      database::DatabaseObject* player = dbrefToObject(*(context.db),
                                                         c->dbref);
      if (player == NULL) {
        std::cout << "Player is null" << std::endl;
        continue;
      }


      if (!visible(context, player)) {
        if (canAddHidden(context, player)) {
          ++hiddenCount;
          ++playerCount;
        }
        continue;
      }
      ++playerCount;

      // name
      columns[0].value = player->getProperty("name");
      // flags
      columns[1].value = "";
      // onfor
      columns[2].value =  formatOnFor(currentTime - c->connectTime);
      // idle
      columns[3].value =  formatIdle(currentTime - c->lastCommandTime);
      // class/pos
      columns[4].value = player->getAttribute("class").value;
      // empire
      columns[5].value = "";
      // doing
      columns[6].value = c->doing;

      context.game->sendNetworkMessage(context.descriptor, formatWhoColumn(columns));
    }
    context.game->sendNetworkMessage(context.descriptor, "---");
    context.game->sendNetworkMessage(context.descriptor,
                                     "Hidden Users: " + boost::lexical_cast<std::string>(hiddenCount) +
                                     "    "  +
                                     "Total Users: " + boost::lexical_cast<std::string>(playerCount));
    return true;
  }

}  // namespace omush
