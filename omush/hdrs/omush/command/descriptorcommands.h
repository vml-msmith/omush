///
/// commandfactory.h
///
///

#ifndef OMUSH_HDRS_OMUSH_COMMAND_DESCRIPTORCOMMANDS_H_
#define OMUSH_HDRS_OMUSH_COMMAND_DESCRIPTORCOMMANDS_H_

#include "omush/command/command.h"
#include "omush/command/commandfactory.h"

namespace omush {
  namespace command {
    class CommandQuit : public Command {
     public:
      CommandQuit();
      bool run();
     private:
      static CommandRegister<CommandQuit> reg;
    };

    class CommandWho  : public Command {
     public:
      CommandWho();
      bool run();

     private:
      static CommandRegister<CommandWho> reg;
    };


    class CommandConnect  : public Command {
     public:
      CommandConnect();
      bool run();

     private:
      static CommandRegister<CommandConnect> reg;
    };

    class CommandWelcomeCreate  : public Command {
     public:
      CommandWelcomeCreate();
      bool run();

     private:
      static CommandRegister<CommandWelcomeCreate> reg;
    };

  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_DESCRIPTORCOMMANDS_H_
