///
/// commandfactory.h
///
///

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDFACTORY_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDFACTORY_H_

#include <string>
#include <map>
#include "omush/command/command.h"

namespace omush {
  namespace command {

    struct CommandFactory {
    public:
     static Command* createInstance(std::string const& s);

    protected:
      static CommandFactoryMap * getMap();
      static CommandFactoryMap* commandMap_;
    };

    template<typename T> struct CommandRegister : CommandFactory {
      explicit CommandRegister(std::string const& s) {
        getMap()->insert(std::make_pair(s, &createT<T>));
      }
    };

  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDFACTORY_H_
