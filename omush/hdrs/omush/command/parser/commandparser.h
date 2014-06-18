/**
 * \file commandparser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
#define OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_

#include "omush/command/command.h"
#include "omush/command/matcher/icommandmatcher.h"
#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace omush {
  class CommandParser {
  public:
    void registerMatcher(ICommandMatcher* matcher);
    void registerCommand(ICommand* command);
    ICommand* findCommand(std::string str, CommandContext& context);

  private:
    std::vector<ICommandMatcher*> commandMatchers_;
    CommandList commandList_;
  };

  /*
  struct CommandNode;
  struct CommandNode {
    Command* cmd;
    std::string str;
    std::map<char,CommandNode*> nodes;
    CommandNode() {
      cmd = NULL;
    }
  };

  typedef boost::function<bool(std::string,CommandContext)> CommandMatchCallback;
  /*
  class CommandMatcher {
  public:
    //    void register(Command* cmd) = 0;
    Command *match(std::string input) = 0
  };

  class CommandMatcherAbsolute : public CommandMatcher {
  public:
    void register(Command *cmd) {
      commandDictionary_.insert(std::make_pair(cmd->name(), cmd));
    }

    Command *match(std::string input) {
      if (commandDictionary_.find(input) == commandDictionary_.end()) {
        return NULL;
      }
      return (commandDictionary_.find(name)->second);
    }

  private:
    std::map<std::string,Command*> commandDictionary_;
  };

  class CommandParser {
  public:
    std::vector<CommandMatcher> matchers;
    typedef boost::function<bool(std::string,CommandContext)> CommandMatchCallback;

    void addMatcher(CommandMatcher matcher) {
      commandMatchers.push_back(CommandMatcherAbsolute());
    }
    CommandParser();
    void registerCommandMatchCallback(class callback, class object);

    bool commandMatchAbsolute(std::string input, CommandContext context) {
      Command *cmd = lookupByName(input);
      // Lookup absolute.
      if (cmd != NULL) {
        cmd->run(input, input, context);
        return true;
      }

      return false;
    }

    void registerCommand(Command *cmd);
    bool run(std::string input, CommandContext context);
    Command* lookupByName(std::string name);
    Command* lookupByShortCode(std::string shortCode);
    ~CommandParser();

    virtual void setupCommands() = 0;
    virtual void setupCommandMatchers() = 0;

  protected:
    std::vector<CommandMatchCallback> matchCallbacks_;

    std::vector<Command*> matchCommandByPrefix(std::string);
    CommandNode* getCommandNode(CommandNode *node, std::string str, std::size_t pos);

    CommandNode* nodes_;
    std::map<std::string,Command*> commandDictionary_;
    std::map<std::string, Command*> commandShortCodeDictionary_;
  };
  */
}

#endif  // OMUSH_HDRS_OMUSH_COMMAND_COMMANDPARSER_H_
