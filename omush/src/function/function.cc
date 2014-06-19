#include "omush/function/function.h"

namespace omush {
std::string FunctionIf::run(std::vector<std::string> args) {
  if (args.size() < 2) {
    return "#-1 IF REQUIRES AT LEAST TWO PARAMETERS";
  }

  if (args.size() > 3) {
    return "#-1 IF MAY ONLY HAVE THREE PARAMETERS";
  }

  if (args[0].compare("1") == 0) {
    return executor_->strParse(args[1]);
  }
  if (args.size() == 3) {
    return executor_->strParse(args[2]);
  }

  return "";
}


bool FunctionAdd::shouldEvaluateParameter(int param) {
  return 1;
}

bool FunctionMul::shouldEvaluateParameter(int param) {
  return 1;
}

bool FunctionSub::shouldEvaluateParameter(int param) {
  return 1;
}
bool FunctionIf::shouldEvaluateParameter(int param) {
  if (param == 0)
    return 1;

  return 0;
}

}  // namespace omush
