///
/// command.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_

#include <map>
#include <vector>
#include <string>

namespace omush {
  namespace command {
    class Command {
     public:
      std::string name = "Test";
      virtual void run() = 0;
    };

    template <typename T> Command* createT() { return new T; }

    typedef std::map<std::string, Command*(*)()> MapType;

    struct CommandFactory {
     public:
      static Command* createInstance(std::string const& s) {
        MapType::iterator it = getMap()->find(s);

        if (it == getMap()->end()) {
          return 0;
        }

        Command *c = it->second();
        //        c->run();
        return c;
      }

     protected:
      static MapType * getMap() {
        if (!myMap) {
          myMap = new MapType();
        }

        return myMap;
      }

     protected:
      static MapType* myMap;
    };

    template<typename T> struct CommandRegister : CommandFactory {
      explicit CommandRegister(std::string const& s) {
        getMap()->insert(std::make_pair(s, &createT<T>));
      }
    };

    class CommandQuit : public Command {
     public:
      void run() {
      }
     private:
      static CommandRegister<CommandQuit> reg;
    };

    class CommandWho  : public Command {
     public:
      std::string name = "Who";
      void run() {
      }
     private:
      static CommandRegister<CommandWho> reg;
    };

    class CommandParser {
     public:
      void run(std::string);
      void registerCommand(std::string str) {
        list.push_back(str);
      }
     private:
      std::vector<std::string> list;
    };
  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
