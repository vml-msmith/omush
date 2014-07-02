/**
 * \file functioncreate.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONCREATE_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONCREATE_H_

#include "omush/function/function.h"
#include "omush/colorstring.h"

namespace omush {
  class FunctionCreate : public IFunction {
   public:
    FunctionCreate();
    ColorString run(ColorString str, ArgList args, FunctionContext& context);
   private:
  };
}  // omush

#endif //  OMUSH_HDRS_OMUSH_FUNCTION_FUNCTIONCREATE_H_
