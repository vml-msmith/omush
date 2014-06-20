#include "omush/function/function.h"

namespace omush {
  /*
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
  */
  std::string floatToString(float x) {
    std::string str = boost::str(boost::format("%f") % x);

    trim_right_if(str, boost::is_any_of("0"));
    trim_right_if(str, boost::is_any_of("."));

    return str;
  }

  ColorString processExpression(ColorString str) {
    FunctionContext context;

    context.functions.insert(std::pair<std::string,IFunction*>("if", new MyFunctionIf()));

    // Math
    context.functions.insert(std::pair<std::string,IFunction*>("add", new MyFunctionAdd()));
    context.functions.insert(std::pair<std::string,IFunction*>("sub", new MyFunctionSub()));
    context.functions.insert(std::pair<std::string,IFunction*>("abs", new FunctionAbs()));

    // Util
    context.functions.insert(std::pair<std::string,IFunction*>("ansi", new FunctionAnsi()));

    return findSelfContained(str, context);
  }

  ColorString findSelfContained(ColorString str, FunctionContext& context) {
    const char *c = str.internalString().c_str();

    int start = -1;
    int end = -1;

    int count = 0;
    int pos = 0;
    char next = '\0';
    char current = '\0';

    while (*c) {
      if (next == '\0') {
        if (*c == '[') {
          start = pos;
          next = ']';
          current = *c;
          ++pos;
          *c++;
          continue;
        }
        if (*c == '(') {
          start = pos;
          next = ')';
          current = *c;
          ++pos;
          *c++;
          continue;
        }
      }
      else {
        if (next == *c) {
          if (count == 0) {
            end = pos;
            break;
          }
          else {
            --count;
            ++pos;
            *c++;
            continue;
          }
        }
        else if (current == *c) {
          ++count;
        }
      }
      ++pos;
      *c++;
    }

    if (end == -1) {
      return str;
    }

    int final = (end + 1)- start;
    int begin = start;

    ColorString response = ColorString(str.internalString().substr(start, final));

    if (current == '(') {
      pos = start;
      while (pos >= 0) {
        if (' ' == str.internalString().c_str()[pos]) {
          break;
        }
        --pos;
      }
      ++pos;
      begin = pos;
      final = final + (start -pos);
      response = ColorString(str.internalString().substr(pos, start - pos) + response.internalString());
    }

    ColorString process = ColorString(str.internalString().substr(begin, final));

    if (current != '\0' && end != '-1') {
      ColorString nextProcess = process;
      // std::cout <<  nextProcess << std::endl;
      context.debugDepth++;

      if (current == '[') {

        std::string debug = "#1!  ";
        for (int i = 0; i < context.debugDepth; ++i) {
          debug += "  ";
        }
        debug += process.basicString();
        std::cout << debug << " : " << std::endl;

        nextProcess = process.internalString().substr(1, process.internalString().length() - 2);
        nextProcess = findSelfContained(nextProcess, context);
        std::cout << debug << " => " << nextProcess.basicString() << std::endl;

      }
      else {
        std::string debug = "#2!  ";

        for (int i = 0; i < context.debugDepth; ++i) {
          debug += "  ";
        }
        debug  += process.basicString();
        std::cout << debug << " : " << std::endl;

        std::vector<std::string> SplitVec; // #2: Search for tokens
        std::string splitter = process.internalString();
        boost::split( SplitVec, splitter, boost::is_any_of("("));
        std::string functionStr = SplitVec[0];
        boost::to_lower(functionStr);
        if (context.functions.find(functionStr) != context.functions.end()) {
          nextProcess = context.functions[functionStr]->run(process,
                                                            getFunctionArgs(process),
                                                            context);
        }
        std::cout << debug << " => " << nextProcess.basicString() << std::endl;

      }

      if (nextProcess.basicString().compare(process.basicString()) != 0) {
        std::string replacer = str.internalString();
        boost::replace_first(replacer, process.internalString(), nextProcess.internalString());
        str = ColorString(replacer);
      }

      if (str.basicString().compare(nextProcess.basicString()) != 0) {
        str = ColorString(str.internalString().substr(0,
                                                 nextProcess.internalString().length() + begin) + findSelfContained(
                                                                                                               str.internalString().substr(nextProcess.internalString().length() + begin, str.internalString().length()), context).internalString());
      }

    }
    --(context.debugDepth);
    return ColorString(str);
  }

  std::vector<ColorString> getFunctionArgs(ColorString str) {
    std::vector<ColorString> args;

    const char *c = str.internalString().c_str();
    int start = -1;
    int end = -1;

    int count = 0;
    int gCount = 0;
    int pos = 0;
    std::deque<char> nextC;

    char current = '\0';

    while(*c != '(') {
      *c++;
      ++pos;
    }

    *c++;
    int last = pos + 1;
    ++pos;

    while (*c) {
      if (gCount <= 0) {
        if (',' == *c || ')' == *c) {
          args.push_back(str.internalString().substr(last, pos - last));
          last = pos + 1;
          // Seperate here.
        }
      }


      if (nextC.empty()) {
        if (*c == '[') {
          nextC.push_front(']');
          current = *c;
          ++pos;
          *c++;
          ++gCount;
          continue;
        }
        if (*c == '(') {
          start = pos;
          nextC.push_front(')');
          current = *c;
          ++pos;
          *c++;
          ++gCount;
          continue;
        }
      }
      else {
        if (nextC.front() == *c) {
          if (count == 0) {
            end = pos;
            --gCount;
            nextC.pop_front();
          }
          else {
            nextC.pop_front();
            --count;
            ++pos;
            *c++;
            continue;
          }
        }
        else if (current == *c) {
          ++count;
        }
      }

      ++pos;
      *c++;
    }

    return args;
  }

}  // namespace omush
