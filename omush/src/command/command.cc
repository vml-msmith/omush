///
/// command.cc
///
/// Copyright 2014 Michael Smith
///

#include "omush/command/command.h"
#include "omush/command/descriptorcommands.h"
#include "omush/network/descriptor.h"
#include <boost/algorithm/string.hpp>

#include "omush/database/database.h"
#include "omush/environment.h"


namespace omush {
  namespace command {

    void Command::init(Environment *env,
                       network::Descriptor *d,
                       database::DatabaseObject *obj,
                       std::string command,
                       std::string input) {
      complain_ = "";
      descriptor_ = d;
      input_ = input;
      command_ = command;
      environment_ = env;
    }

    std::string Command::getName() {
      return this->name_;
    }

    AliasList Command::getAlias() {
      AliasList v;
      return v;
    }


  }  // namespace command
}  // namespace omus
