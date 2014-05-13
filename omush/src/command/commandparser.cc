///
/// commandparser.cc
///
/// Copyright Michael Smith 2014.
///

#include "omush/command/commandparser.h"

#include <boost/algorithm/string.hpp>

#include "omush/command/command.h"
#include "omush/command/commandfactory.h"

namespace omush {
  namespace command {
    void CommandParser::registerEnvironment(Environment& env) {
      environment_ = &env;
    }

    CommandParser::~CommandParser() {
      for (CommandMap::iterator i = list.begin();
           i != list.end(); ++i) {
        delete i->second;
      }
      list.clear();
    }

    bool CommandParser::run(network::Descriptor *d,
                            database::DatabaseObject *obj,
                            std::string str) {
      boost::trim(str);
      std::string input = str;

      unsigned int spacePos = str.find(' ');
      if (spacePos != std::string::npos) {
        str = str.substr(0, spacePos);
      }
      boost::to_upper(str);
      Command *c = lookupByName(str);
      if (c == NULL) {
        c = lookupByAlias(str);
      }

      if (c == NULL) {
        return false;
      }
      c->init(environment_, d, obj, str, input);
      c->run();
      return true;
    }

    Command* CommandParser::lookupByName(std::string str) {
      if (namedList_.count(str)) {
        return namedList_[str];
      }
      return NULL;
    }

    Command* CommandParser::lookupByAlias(std::string str) {
      if (aliasList_.count(str)) {
        return aliasList_[str];
      }
      return NULL;
    }

    void CommandParser::registerCommand(std::string str) {
      Command *c = CommandFactory::createInstance(str);
      list.insert(std::make_pair(str, c));
      namedList_.insert(std::make_pair(c->getName(), c));
      for (std::string i : c->getAlias()) {
        aliasList_.insert(std::make_pair(i, c));
      }
    }

  }  // namespace command
}  // namespace omush
