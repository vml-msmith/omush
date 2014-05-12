///
/// command.cc
///
/// Copyright 2014 Michael Smith
///

#include "omush/command/command.h"

namespace omush {
  namespace command {
    MapType* CommandFactory::myMap = new MapType();

    CommandRegister<CommandQuit> CommandQuit::reg("CommandQuit");
    CommandRegister<CommandWho>  CommandWho::reg("CommandWho");

    void CommandParser::run(std::string str) {
      for (std::vector<std::string>::iterator i = list.begin();
           i != list.end(); ++i) {
        Command *c = CommandFactory::createInstance(*i);
        c->run();
        delete c;
      }
    }
  }  // namespace command
}  // namespace omush
