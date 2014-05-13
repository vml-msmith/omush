///
/// descriptorcommands.cc
///
/// Copyright Michael Smith 2014

#include "omush/command/descriptorcommands.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "omush/network/descriptor.h"
#include "omush/command/commandfactory.h"
#include "omush/database/database.h"
#include "omush/environment.h"

namespace omush {
  namespace command {

    CommandQuit::CommandQuit() { name_ = "QUIT"; }

    bool CommandQuit::run() {
      return true;
    }

    CommandWho::CommandWho() { name_ = "WHO"; }

    bool CommandWho::run() {
      std::cout << "Who from " << command_ << std::endl;
      return true;
    }

    CommandConnect::CommandConnect()  { name_ = "CONNECT"; }

    bool CommandConnect::run() {
      std::vector<std::string> parts;
      boost::split(parts, input_, boost::is_any_of(" "));

      if (parts.size() > 3) {
        std::cout << "Too many args." << std::endl;
      } else if (parts.size() < 2) {
        std::cout << "Too few args." << std::endl;
      }
      std::string name = parts[1];
      std::string pass = "";
      if (parts.size() == 3) {
        pass = parts[2];
      }

      database::Player* p = database::Player::findByNameAndPass(environment_->database, name, pass);
      if (p == NULL) {
        // Show some message for fail? I dunno.
        return false;
      }

      descriptor_->doLogin(p);
      return true;
    }

    CommandWelcomeCreate::CommandWelcomeCreate()  {  name_ = "CREATE"; }

    bool CommandWelcomeCreate::run() {
      std::vector<std::string> parts;
      boost::split(parts, input_, boost::is_any_of(" "));

      if (parts.size() > 3) {
        // TODO(msmith): Replace any strings with constants.
        descriptor_->sendMessage("Fail: Too many");
        return false;
      } else if (parts.size() < 3) {
        // TODO(msmith): Replace any strings with constants.
        descriptor_->sendMessage("Fail: Too few");
        return false;
      }
/*
      std::string name = parts[1];
      std::string pass = parts[2];

      database::Player* p = database::Player::findByName(environment_->database,
                                                         name);
      if (p != NULL) {
        // TODO(msmith): Replace any strings with constants.
        std::string out;
        out = "There is already a player with that name. Please try another one.");
        descriptor_->sendMessage(out);
        return false;
      }

      p = database::Player::createNewPlayer(name, pass);
*/
      return true;
    }

    CommandRegister<CommandQuit>           CommandQuit::reg("CommandQuit");
    CommandRegister<CommandWho>            CommandWho::reg("CommandWho");
    CommandRegister<CommandConnect>        CommandConnect::reg("CommandConnect");
    CommandRegister<CommandWelcomeCreate>  CommandWelcomeCreate::reg("CommandWelcomeCreate");

  }  // namespace command
}  // namespace omush
