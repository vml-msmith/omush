/**
 * \file parser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_FUNCTION_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_FUNCTION_H_

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "omush/function/engine/lexxer.h"
#include "omush/function/engine/parser.h"
#include "omush/function/engine/executor.h"

namespace omush {
class Function {
public:
  const std::string NOT_A_NUMBER = "#-1 ARGUMENTS MUST BE NUMBERS";
  virtual std::string run(std::vector<std::string> args) = 0;
  virtual bool shouldEvaluateParameter(int param) = 0;

 Function(Executor* e)  : executor_(e) {}

 protected:
  Executor* executor_;
};


class FunctionAdd : public Function {
public:
 FunctionAdd(Executor* p) : Function(p) {}

  std::string run(std::vector<std::string> args) {
   int result = 0;
    for (std::vector<std::string>::iterator it = args.begin();
         it != args.end();
         ++it) {

      std::string val = *it;
      boost::trim(val);

      try {
        result += boost::lexical_cast<int>(val);
      } catch (boost::bad_lexical_cast&) {
        //        return NOT_A_NUMBER;
      }
    }

    return boost::lexical_cast<std::string>(result);
  }
  bool shouldEvaluateParameter(int param);
};

class FunctionRepeat : public Function {
 public:
  FunctionRepeat(Executor* p) : Function(p) {}

  std::string run(std::vector<std::string> args) {
    if (args.size() != 2) {
      return "#-1 FUNCTION (REPEAT) EXPECTS 2 ARGUMENTS BUT GOT "
        + boost::lexical_cast<std::string>(args.size());
    }

    int count = 0;
    try {
      count = boost::lexical_cast<int>(args[1]);
    } catch (boost::bad_lexical_cast&) {
      return "#-1 ARGUMENT MUST BE INTEGER";
    }

    if (count < 0) {
      return "#-1 ARGUMENT MUST BE NON-NEGATIVE INTEGER";
    }


    std::string str = "";
    for (int i = 0; i < count; ++i)
      str += args[0];

    return str;
  }
  bool shouldEvaluateParameter(int param) {
    return true;
  }
};

class FunctionSub : public Function {
public:
 FunctionSub(Executor* p)  : Function(p) {}
  std::string run(std::vector<std::string> args) {
   int result = 0;
   bool started = false;
   for (std::vector<std::string>::iterator it = args.begin();
        it != args.end();
        ++it) {

     try {
       if (!started) {
         result = boost::lexical_cast<int>(*it);
         started = true;
         continue;
       }

       result -= boost::lexical_cast<int>(*it);
      } catch (boost::bad_lexical_cast&) {
        return NOT_A_NUMBER;
      }
   }

    return boost::lexical_cast<std::string>(result);
  }
  bool shouldEvaluateParameter(int param);
};

class FunctionMul : public Function {
public:
 FunctionMul(Executor* p)  : Function(p) {}
  std::string run(std::vector<std::string> args) {
    int result = 1;

    for (std::vector<std::string>::iterator it = args.begin();
         it != args.end();
         ++it) {
      try {
        result *= boost::lexical_cast<int>(*it);
      } catch (boost::bad_lexical_cast&) {
        return NOT_A_NUMBER;
      }
    }

    return boost::lexical_cast<std::string>(result);
  }
  bool shouldEvaluateParameter(int param);
};

class FunctionIf : public Function {
 public:
 FunctionIf(Executor* e)  : Function(e) {}
  bool shouldEvaluateParameter(int param);
  std::string run(std::vector<std::string> args);
};


class FunctionExecutor : public Executor {
public:
  FunctionExecutor() {

    funcs["add"] = new FunctionAdd(this);
    funcs["sub"] = new FunctionSub(this);
    funcs["mul"] = new FunctionMul(this);
    funcs["if"] = new FunctionIf(this);
    funcs["repeat"] = new FunctionRepeat(this);
  }

  ~FunctionExecutor() {
    for (std::map<std::string,Function*>::iterator iter = funcs.begin(); iter != funcs.end(); ++iter) {
      delete iter->second;
    }
  }

  std::string strParse(std::string input) {
    Lexxer l = Lexxer();
    l.lex(input);

    std::deque<std::pair<TokenType,std::string> > tree = l.tree();

    Parser p = Parser();
    p.parse(l);
    if (l.tree().size() > 1) {
      FunctionExecutor e = FunctionExecutor();
      return e.execute(p);
    }
    return input;
  }
};

}

#endif
