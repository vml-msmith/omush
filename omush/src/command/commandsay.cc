/**
 * \file commandsay.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/commandsay.h"
#include <boost/algorithm/string.hpp>
#include "omush/database/database.h"
#include "omush/action.h"

namespace omush {

std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep, int segments) {
std::vector<std::string> result;
int i = 0;
std::size_t pos = str.find(sep);
while (i < (segments - 1) && pos != std::string::npos) {
std::cout << "Seg: " << str.substr(0,pos) << std::endl;
result.push_back(str.substr(0,pos));
std::cout << "still" << std::endl;
str = str.substr(pos + 1, str.length() - pos + 1);
std::cout << "Str: " << str << std::endl;
++i;
}
result.push_back(str);
return result;
}

  CommandSay::CommandSay() {
    name_ = "SAY";
    absolute_ = false;
  }

  bool CommandSay::run(std::string calledAs, std::string input, CommandContext context) {
    Command::run_(calledAs, input, context);
std::string words = "";
    std::vector<std::string> inputParts = splitStringIntoSegments(input, " ", 2);
if (inputParts.size() > 1) {
words = inputParts[1];
}

ActionSay(context.db, context.game, context.db->findObjectByDbref(context.dbref)).what(words).enact();
    return true;
  }

}  // namespace omush
