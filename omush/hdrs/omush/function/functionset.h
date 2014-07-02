/**
 * \file functionset.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONSET_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONSET_H_

#include "omush/function/function.h"
#include "omush/colorstring.h"

namespace omush {
  class FunctionSet : public IFunction {
   public:
    FunctionSet();
    ColorString run(ColorString str, ArgList args, FunctionContext& context);
   private:
    void setAttribute(CommandContext& context,
                      database::DatabaseObject* target,
                      std::string attribute,
                      std::string value);
    void setFlag(CommandContext& context,
                 database::DatabaseObject* target,
                 std::string flagName);
  };
}  // omush

#endif //  OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONSET_H_
