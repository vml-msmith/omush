#include "omush/command/command.h"

namespace omush {
  namespace command {

    CommandRegister<CommandQuit> CommandQuit::reg("CommandQuit");
    CommandRegister<CommandWho>  CommandWho:reg("CommandWho");
    
    void CommandParser::run(std::string str) {
        /*
        for (std::vector<std::string>::iterator i = list.begin();
             i != list.end(); ++i) {
          Command *c = CommandFactory::createInstance(*i);
          c->run();
          delete c;
        }
        */
    }
  }
}
