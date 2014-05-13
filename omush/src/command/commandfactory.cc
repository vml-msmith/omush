///
/// commandfactory.cc
///
/// Copyright Michael Smith 2014

#include "omush/command/commandfactory.h"
#include "omush/command/command.h"

namespace omush {
  namespace command {
    CommandFactoryMap* CommandFactory::commandMap_ = new CommandFactoryMap();

    Command* CommandFactory::createInstance(std::string const& s) {
      CommandFactoryMap::iterator it = getMap()->find(s);

      if (it == getMap()->end()) {
        return NULL;
      }

      Command *c = it->second();
      return c;
    }

    CommandFactoryMap* CommandFactory::getMap() {
      if (!commandMap_) {
        commandMap_ = new CommandFactoryMap();
      }

      return commandMap_;
    }
  }  // namespace command
}  // namespace omush
