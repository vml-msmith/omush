#include "omush/function/function.h"
#include "omush/database/databaseobject.h"
#include "omush/database/utilityfactories.h"

#include "omush/function/functioncreate.h"
#include "omush/function/functionset.h"
#include "omush/command/commandcontext.h"

namespace omush {

  std::string floatToString(float x) {
    std::string str = boost::str(boost::format("%f") % x);

    trim_right_if(str, boost::is_any_of("0"));
    trim_right_if(str, boost::is_any_of("."));

    return str;
  }

  ColorString processExpression(ColorString str, FunctionScope* scope) {
    return processExpression(str, scope, -1);
  }

  ColorString processExpression(ColorString str, FunctionScope* scope, int limit) {
    FunctionContext context;
    context.limit = limit;
    context.scope = scope;

    // Database
    context.functions.insert(std::pair<std::string,IFunction*>("create", new FunctionCreate()));
    context.functions.insert(std::pair<std::string,IFunction*>("set", new FunctionSet()));

    // Programming
    context.functions.insert(std::pair<std::string,IFunction*>("if", new MyFunctionIf()));

    // Math
    context.functions.insert(std::pair<std::string,IFunction*>("add", new MyFunctionAdd()));
    context.functions.insert(std::pair<std::string,IFunction*>("sub", new MyFunctionSub()));
    context.functions.insert(std::pair<std::string,IFunction*>("abs", new FunctionAbs()));

    // Util
    context.functions.insert(std::pair<std::string,IFunction*>("ansi", new FunctionAnsi()));

    // Str
    context.functions.insert(std::pair<std::string,IFunction*>("ucstr", new FunctionUcstr()));
    std::cout << "Before proc" << std::endl;
    ColorString retStr = findSelfContained(str, context);
    std::cout << "after proc" << std::endl;
    return retStr;
  }

  std::string capstr(std::string str) {
    char first = str[0];
    toupper(first);

    str = first + str.substr(1,str.length() - 1);
    return str;
  }

  ColorString runReplacements(ColorString str, FunctionContext& context) {
    database::DatabaseAttribute sexAttr = context.scope->enactor->getAttribute("sex");

    std::string enactorName = nameFormatter(*(context.scope->cmdContext->db), context.scope->enactor).formatInline(context.scope->enactor);
    std::string enactorSubPronoun = "it";
    std::string enactorObjPronoun = "it";
    std::string enactorPosPronoun = "its";
    std::string enactorAbsPronoun = "it";
    if (boost::iequals(sexAttr.value, "male")) {
      enactorSubPronoun = "he";
      enactorObjPronoun = "him";
      enactorPosPronoun = "his";
      enactorAbsPronoun = "his";
    }
    else if (boost::iequals(sexAttr.value, "female")) {
      enactorSubPronoun = "she";
      enactorObjPronoun = "her";
      enactorPosPronoun = "her";
      enactorAbsPronoun = "hers";
    }


    std::string internal = str.internalString();


    boost::replace_first(internal, "%r", "\n");
    boost::replace_first(internal, "%R", "\n");
    boost::replace_first(internal, "%b", " ");
    boost::replace_first(internal, "%t", "    ");
    boost::replace_first(internal, "%#", nameFormatter(*(context.scope->cmdContext->db), context.scope->enactor).formatDbref(context.scope->enactor->dbref()));
    boost::replace_first(internal, "%n", enactorName);
    boost::replace_first(internal, "%N", capstr(enactorName));
    boost::replace_first(internal, "%s", enactorSubPronoun);
    boost::replace_first(internal, "%S", capstr(enactorSubPronoun));
    boost::replace_first(internal, "%o", enactorObjPronoun);
    boost::replace_first(internal, "%O", capstr(enactorObjPronoun));
    boost::replace_first(internal, "%p", enactorPosPronoun);
    boost::replace_first(internal, "%P", capstr(enactorPosPronoun));
    boost::replace_first(internal, "%a", enactorAbsPronoun);
    boost::replace_first(internal, "%A", capstr(enactorAbsPronoun));
    boost::replace_first(internal, "%@", nameFormatter(*(context.scope->cmdContext->db), context.scope->caller).formatDbref(context.scope->caller->dbref()));
    boost::replace_first(internal, "%!", nameFormatter(*(context.scope->cmdContext->db), context.scope->executor).formatDbref(context.scope->executor->dbref()));

    return ColorString(internal);
  }

  ColorString findSelfContained(ColorString str, FunctionContext& context) {
    const char *c = str.internalString().c_str();
    std::cout << "Process " << str.internalString() << std::endl;
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
      str = runReplacements(str, context);
      return str;
    }

    if (start > 0 && context.limit > 0) {
      const char *s = str.internalString().c_str();
      for (int i = 0; i < start; ++i) {
        if (*s == ' ') {
          str = runReplacements(str, context);
          return str;
        }
        *s++;
      }
    }

    int final = (end + 1) - start;
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
      context.debugDepth++;

      if (current == '[') {

        std::string debug = "#1!  ";
        for (int i = 0; i < context.debugDepth; ++i) {
          debug += "  ";
        }
        debug += process.basicString();

        std::string str_one = process.internalString();

        nextProcess = ColorString(str_one.substr(1, str_one.length() - 2));
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

      if (str.basicString().compare(nextProcess.basicString()) != 0 && context.limit < 0) {
        str = ColorString(str.internalString().substr(0,
                                                 nextProcess.internalString().length() + begin) + findSelfContained(
                                                                                                               str.internalString().substr(nextProcess.internalString().length() + begin, str.internalString().length()), context).internalString());
      }

    }
    // Run replacements on str.
    std::cout << "HEREEEEE" << std::endl;
    str = runReplacements(str, context);
    std::cout << "222222E" << std::endl;
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
