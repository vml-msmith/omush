/**
 * \file icommand.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_ICOMMAND_H_
#define OMUSH_HDRS_OMUSH_COMMAND_ICOMMAND_H_

#include <string>
#include <vector>
#include <map>
#include "omush/command/commandcontext.h"

namespace omush {
  /**
   * Interface for all commands useable by objects in the game. Every
   * action in the game should be setup by an action of some sort.
   */
  class ICommand {
   public:
    /**
     * Set the name of the command.
     *
     * @param name  String of the first word of command to match, such as @dig
     *              or CONNECT.
     */
    explicit ICommand(std::string name) : name_(name) {}

    /**
     * Execute the command.
     *
     * @param context  The context of the command, including the game, client,
     *                 DB and any scope. Scope is executor, enactor, caller and
     *                 function scope.
     */
    virtual bool run(CommandContext& context) = 0;

    /**
     * Return the name of the command.
     */
    std::string name() { return name_; }

    /**
     * Send a notification to the executor via context. The notification is sent
     * via the Game in context.
     */
    void notifyExecutor(CommandContext& context, std::string msg);

   private:
    std::string name_;
  };

  typedef std::vector<ICommand*> CommandList;
  typedef std::map<std::string, ICommand*> CommandMap;

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COMMAND_ICOMMAND_H_
