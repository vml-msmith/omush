/**
 * \file commandwho.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDWHO_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDWHO_H_

#include <string>
#include <boost/lexical_cast.hpp>
#include "omush/command/icommand.h"

namespace omush {
  struct WhoColumn {
    size_t length;
    std::string value;
    WhoColumn(std::string val, size_t size) : length(size), value(val) {}
  };

  /**
   */
  class CommandWho : public ICommand {
   public:
    CommandWho();
    bool run(CommandContext& context);
   private:
    CommandInfo process(CommandContext& context);
    std::string formatWhoColumn(std::vector<WhoColumn> columns);
    std::string formatOnFor(boost::posix_time::time_duration time);
    std::string formatIdle(boost::posix_time::time_duration time);
    bool canAddHidden(CommandContext& context, database::DatabaseObject* player);
    bool visible(CommandContext& context, database::DatabaseObject* player);
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDWHO_H_
