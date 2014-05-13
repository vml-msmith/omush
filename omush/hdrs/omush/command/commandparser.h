///
/// commandfactory.h
///
///

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_

#include <string>

#include "omush/command/command.h"

namespace omush {
  namespace command {
    class CommandParser {
     public:
      bool run(network::Descriptor *d,
               database::DatabaseObject *obj,
               std::string str);
      void registerEnvironment(Environment& env);
      void registerCommand(std::string str);
      ~CommandParser();

     private:
      Command* lookupByName(std::string str);
      Command* lookupByAlias(std::string);

      Environment *environment_;
      CommandMap list;
      CommandMap namedList_;
      CommandMap aliasList_;
    };
  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
