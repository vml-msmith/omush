/**
 * \file commandparser.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/command/parser/commandparser.h"
#include "omush/command/command.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "omush/database/database.h"
#include "omush/database/databaseobject.h"
#include "omush/database/targetmatcher.h"
#include <boost/scoped_ptr.hpp>
#include "omush/command/commandgo.h"
#include "omush/utility.h"

namespace omush {
  void CommandParser::registerMatcher(ICommandMatcher* matcher) {
    commandMatchers_.push_back(matcher);
  }

  void CommandParser::registerCommand(ICommand* command) {
    commandList_.push_back(command);
  }

  ICommand* CommandParser::findCommand(std::string cmd,
                                       CommandContext& context) {
    context.modifiedInput = cmd;
    BOOST_FOREACH(ICommandMatcher* matcher, commandMatchers_) {
      ICommand* c = NULL;
      if (matcher->hasContextMatch()) {
        c = matcher->contextMatchByString(commandList_, context, cmd);
      }
      else {
        if (matcher->isCached())
          c = matcher->matchByString(cmd);
        else
          c = matcher->matchByString(commandList_, cmd);
      }

      if (c != NULL) {
        return c;
      }
    }

    return NULL;
  }

}  // namespace omush
