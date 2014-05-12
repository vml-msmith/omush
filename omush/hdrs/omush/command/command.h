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
#include <iostream>
#include "omush/network/descriptor.h"

namespace omush {
  namespace command {
    class Command {
     public:
      std::string name_;
      void init(network::Descriptor *d, std::string command, std::string input);
      virtual void run() = 0;
      std::string getName() { return this->name_; }
      std::vector<std::string> getAlias() {  std::vector<std::string> v; return v;}

    protected:
      std::string input_;
      std::string command_;
      network::Descriptor *descriptor_;
      std::string complain_;
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
      void run() {}

      std::string getName() { return "QUIT"; }
      std::vector<std::string> getAlias() {  std::vector<std::string> v; return v;}

     private:
      static CommandRegister<CommandQuit> reg;
    };

    class CommandWho  : public Command {
     public:
      CommandWho() { name_ = "WHO"; }
      void run();

     private:
      static CommandRegister<CommandWho> reg;
    };

    class CommandParser {
     public:
      void run(std::string);
      void registerCommand(std::string str);
      ~CommandParser() {
        for (std::map<std::string, Command*>::iterator i = list.begin();
             i != list.end(); ++i) {
          delete i->second;
        }
        list.clear();
      }

     private:
      Command* lookupByName(std::string str);
      Command* lookupByAlias(std::string);

      std::map<std::string, Command*> list;
      std::map<std::string, Command*> namedList_;
      std::map<std::string, Command*> aliasList_;
    };

    
  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
