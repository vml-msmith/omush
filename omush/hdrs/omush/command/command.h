///
/// command.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_

#include <vector>
#include <string>
#include <map>

namespace omush {
  class Environment;

  namespace database {
    class DatabaseObject;
  }  // namespace database

  namespace network {
    class Descriptor;
  }  // namespace network

  namespace command {
    typedef std::vector<std::string> AliasList;

    class Command {
     public:
      void init(Environment *env,
                network::Descriptor *d,
                database::DatabaseObject *obj,
                std::string command,
                std::string input);

      virtual bool run() = 0;
      std::string getName();
      AliasList getAlias();

    protected:
      std::string name_;
      std::string input_;
      std::string command_;
      network::Descriptor *descriptor_;
      std::string complain_;
      Environment *environment_;
    };

    template <typename T> Command* createT() { return new T; }

    typedef std::map<std::string, Command*(*)()> CommandFactoryMap;
    typedef std::map<std::string, Command*> CommandMap;

  }  // namespace command
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMAND_H_
