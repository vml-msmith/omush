///
/// command.cc
///
/// Copyright 2014 Michael Smith
///

#include "omush/command/command.h"
#include <boost/algorithm/string.hpp>

namespace omush {
  namespace command {
    MapType* CommandFactory::myMap = new MapType();

    CommandRegister<CommandQuit> CommandQuit::reg("CommandQuit");
    CommandRegister<CommandWho>  CommandWho::reg("CommandWho");

    void CommandParser::run(std::string str) {
      boost::trim(str);
      std::string input = str;
   
      unsigned int spacePos = str.find(' ');
      if (spacePos != std::string::npos) {
        str = str.substr(0, spacePos);
      }
      boost::to_upper(str);
      Command *c = lookupByName(str);
      if (c == NULL) {
        c = lookupByAlias(str);
      }

      if (c == NULL) {
        return;
      }
      c->init(NULL, str, input);
      c->run();
    }

    Command* CommandParser::lookupByName(std::string str) {
      std::cout << "Find " << str << std::endl;
      if (namedList_.count(str)) {
        std::cout << "Found!" << std::endl;
        return namedList_[str];
      }
      return NULL;
    }

    Command* CommandParser::lookupByAlias(std::string str) {
      if (aliasList_.count(str)) {
        return aliasList_[str];
      }
      return NULL;
    }

    void CommandParser::registerCommand(std::string str) {
      Command *c = CommandFactory::createInstance(str);
      list.insert(std::make_pair(str, c));
      namedList_.insert(std::make_pair(c->getName(), c));
      for (std::string i : c->getAlias()) {
        aliasList_.insert(std::make_pair(i, c));
      }
    }

    void CommandWho::run() {
      std::cout << "Who from " << command_ << std::endl;
    }

    void Command::init(network::Descriptor *d, std::string command, std::string input) {
      complain_ = "";
      descriptor_ = d;
      input_ = input;
      command_ = command;
    }

  }  // namespace command
}  // namespace omush
