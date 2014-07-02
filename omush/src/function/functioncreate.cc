/**
 * \file functioncreate.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/function/functioncreate.h"
#include "omush/action/actioncreate.h"
#include "omush/database/utilityfactories.h"

namespace omush {
  FunctionCreate::FunctionCreate()
    : IFunction() { maxArgs = 1; minArgs = 1; name="create"; }

  ColorString FunctionCreate::run(ColorString str, ArgList args, FunctionContext& context) {
    FunctionState state = preProcess(str, args, context);
    if (state.error)
      return ColorString(state.errorString);

    std::string name = findSelfContained(args[0], context).basicString();
    boost::trim(name);

    ActionCreate create(*(context.scope->cmdContext));
    create.name(name);
    create.enact();

    if (create.newObject == NULL) {
      return ColorString("#-1");
    }

    return ColorString(nameFormatter(*(context.scope->cmdContext->db), NULL)
                       .formatDbref(create.newObject->dbref()));
  };

}  // namespace omush
