/**
 * \file functionset.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/function/functionset.h"
#include "omush/action/actionsetattribute.h"
#include "omush/action/actionsetflag.h"
#include "omush/nameformatter.h"
#include "omush/database/targetmatcher.h"

namespace omush {
  FunctionSet::FunctionSet()
    : IFunction() { maxArgs = 2; minArgs = 2; name="set"; }

  ColorString FunctionSet::run(ColorString str, ArgList args, FunctionContext& context) {
    FunctionState state = preProcess(str, args, context);
    if (state.error)
      return ColorString(state.errorString);

    std::string objectString = findSelfContained(args[0], context).basicString();
    std::string valueString = findSelfContained(args[0], context).basicString();

    boost::trim(objectString);
    boost::trim(valueString);
    std::cout << "MEh" << std::endl;
    std::vector<database::DatabaseObject*> matches;
    database::TargetMatcher matcher = database::TargetMatcher(context.scope->cmdContext->db,
                                                              context.scope->cmdContext->cmdScope.executor);
    std::cout << "MEhe" << std::endl;
    matches = matcher.match(objectString);
    std::cout << "ME3h" << std::endl;
    if (matches.size() == 0) {
      //      notifyExecutor(context, "I can't see that here.");
      return ColorString("");
    }

    if (matches.size() > 1) {
      //      notifyExecutor(context, "I don't know which one you mean.");
      return ColorString("");
    }
    std::cout << "MEh5" << std::endl;
    // Handle ":"
    std::vector<std::string> colonSplit = splitStringIntoSegments(valueString, ":", 2);
    if (colonSplit[0].length() > 0) {
    std::cout << "MEh5" << std::endl;
      std::vector<std::string> spaceSplit = splitStringIntoSegments(colonSplit[0], " ", 2);
    std::cout << "MEhf" << std::endl;
      if (spaceSplit.size() == 1) {
    std::cout << "aMEh" << std::endl;
        // Do attribute set.
        setAttribute(*(context.scope->cmdContext),
                     matches[0],
                     colonSplit[0],
                     colonSplit[1]);
    std::cout << "MEdh" << std::endl;
        return ColorString("");
      }
    }
    std::cout << "CRRMEh" << std::endl;
    // Do flag set.
    setFlag(*(context.scope->cmdContext),
            matches[0],
            valueString);

    return ColorString("");
  }

  void FunctionSet::setAttribute(CommandContext& context,
                                 database::DatabaseObject* target,
                                 std::string attribute,
                                 std::string value) {
    ActionSetAttribute(context)
      .target(target)
      .attribute(attribute)
      .value(value)
      .enact();
    return;
  }

  void FunctionSet::setFlag(CommandContext& context,
                           database::DatabaseObject* target,
                           std::string flagName) {
    bool unset = false;
    const char *firstChar = flagName.c_str();

    if (*firstChar == '!' && flagName.length() > 1) {
      unset = true;
      flagName = flagName.substr(1, flagName.length());
    }

    Flag* flag = context.db->flags.getFlag(flagName);
    if (flag == NULL) {
      //      notifyExecutor(context, "I don't know that flag.");
      return;
    }
    ActionSetFlag action = ActionSetFlag(context);
    action.target(target);
    action.flag(flag);

    if (unset) {
      action.unset();
    }

    action.enact();

    return;
  }

}  // namespace omush
