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
#include <boost/format.hpp>
#include <cmath>
#include "omush/utility.h"
#include "omush/colorstring.h"
#include "omush/database/definitions.h"


namespace omush {
  std::string floatToString(float x);
  typedef std::vector<ColorString> ArgList;

  struct FunctionContext;
  struct FunctionState {
    bool error;
    std::string errorString;
  };


  class IFunction {
  public:
  IFunction() : maxArgs(-1), minArgs(-1) { }
    virtual ColorString run(ColorString str, ArgList args, FunctionContext& context) = 0;

  protected:
    FunctionState preProcess(ColorString str,
                             ArgList args,
                             FunctionContext& context) {
      FunctionState state;
      state.error = false;

      if (minArgs >= 0) {
        if (args.size() < minArgs) {
          state.error = true;
          state.errorString = "#-1 FUNCTION (" + name + ")" +
            " EXPECTS AT LEAST " + boost::lexical_cast<std::string>(minArgs) +
            " ARGUMENTS BUT GOT " + boost::lexical_cast<std::string>(args.size());
        }
      }

      if (!state.error && maxArgs > -1) {
        if (args.size() > maxArgs) {
          state.error = true;
          state.errorString = "#-1 FUNCTION (" + name + ")" +
            " EXPECTS BETWEEN " + boost::lexical_cast<std::string>(minArgs) +
            " AND " + boost::lexical_cast<std::string>(minArgs) +  " ARGUMENTS" +
            " BUT GOT " + boost::lexical_cast<std::string>(args.size());
        }
      }

      boost::to_upper(state.errorString);
      return state;
    }

    std::string name;
    int maxArgs = -1;
    int minArgs = -1;
  };


  typedef std::map<std::string, IFunction*> FunctionMap;


  struct FunctionScope {
    database::DatabaseObject* enactor;
    database::DatabaseObject* executor;
    database::DatabaseObject* caller;
    //    FunctionScope(database::Dbref dbref) :  enactor(dbref), executor(dbref), caller(dbref) {}
    FunctionScope() {}
  };

  struct FunctionContext {
    FunctionMap functions;
    int debugDepth;
    int limit;
    FunctionScope* scope;

    FunctionContext() : debugDepth(0) { }
  };

  ColorString findSelfContained(ColorString str, FunctionContext& context);

  class MyFunctionAdd : public IFunction {
  public:
  MyFunctionAdd() : IFunction() { maxArgs = -1; minArgs = 2; name="add"; }

    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      float result = 0;

      for (std::vector<ColorString>::iterator it = args.begin();
           it != args.end();
           ++it) {

        ColorString val = *it;

        val = findSelfContained(val, context);
        std::string number = val.basicString();
        boost::trim(number);
        try {
          result += boost::lexical_cast<float>(number);
        } catch (boost::bad_lexical_cast&) {
          //        return NOT_A_NUMBER;
        }
      }


      return ColorString(floatToString(result));
    }
  protected:
  };

  class MyFunctionSub : public IFunction {
  public:
  MyFunctionSub()  : IFunction() { name="sub"; }
    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      float result = 0;
      bool started = false;
      for (std::vector<ColorString>::iterator it = args.begin();
           it != args.end();
           ++it) {

        ColorString val = *it;

        val = findSelfContained(val, context);
        std::string number = val.basicString();
        boost::trim(number);

        try {
          if (!started) {
            try {
              result += boost::lexical_cast<float>(number);
            } catch (boost::bad_lexical_cast&) {
              return ColorString("#-1 NOT A NUMBER");
            }

            started = true;
            continue;
          }

          try {
            result -= boost::lexical_cast<float>(number);
          } catch (boost::bad_lexical_cast&) {
            return ColorString("#-1 NOT A NUMBER");
          }

        } catch (boost::bad_lexical_cast&) {
          return ColorString("#-1 NOT A NUMBER");
        }
      }


      return ColorString(floatToString(result));
    }

  };

  class MyFunctionIf : public IFunction {
   public:
    MyFunctionIf()  : IFunction() { maxArgs = 3; minArgs = 2; name="if"; }
    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      std::string op = findSelfContained(args[0], context).basicString();
      boost::trim(op);
      if (op.compare("1") == 0) {
        return findSelfContained(args[1], context);
      }
      if (args.size() == 3) {
        return findSelfContained(args[2], context);
      }
    }
  };

  class FunctionAbs : public IFunction {
  public:
  FunctionAbs()  : IFunction() { maxArgs = 1; minArgs = 1; name="abs"; }
    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      float result;
      ColorString val = findSelfContained(ColorString(args[0]), context);
      std::string number = val.basicString();
      boost::trim(number);

      try {
        result = std::abs(boost::lexical_cast<float>(number));
      } catch (boost::bad_lexical_cast&) {
        return ColorString("#-1 ARGUMENT MUST BE A NUMBER");
      }

      return  ColorString(floatToString(result));
    }

  };

  class FunctionAnsi : public IFunction {
  public:
  FunctionAnsi()  : IFunction() { maxArgs = 2; minArgs = 2; name="ansi"; }
    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      std::string op = findSelfContained(args[0], context).basicString();
      boost::trim(op);

      std::queue<std::string> colors;

      for (int i = 0; i < op.length(); ++i) {
        switch (op.c_str()[i]) {
        case 'r':
          colors.push("red");
          break;
        case 'g':
          colors.push("green");
          break;
        case 'w':
          colors.push("white");
          break;
        case 'm':
          colors.push("magenta");
          break;
        case 'c':
          colors.push("cyan");
          break;
        case 'x':
          colors.push("black");
          break;
        }
      }


      ColorString output = findSelfContained(args[1], context);
      while (!colors.empty()) {
        output = ColorString(ColorString::color(output.internalString(), colors.front()));
        colors.pop();
      }
      return output;
    }

  };

  class FunctionUcstr  : public IFunction {
  public:
  FunctionUcstr()  : IFunction() { maxArgs = 1; minArgs = 1; name="ucstr"; }
    ColorString run(ColorString str, ArgList args, FunctionContext& context) {
      FunctionState state = preProcess(str, args, context);
      if (state.error)
        return ColorString(state.errorString);

      ColorString output = findSelfContained(args[0], context);

      output.to_upper();

      return output;
    }

  };



  std::vector<ColorString> getFunctionArgs(ColorString str);

  ColorString findSelfContained(ColorString str, FunctionContext& context);

  ColorString processExpression(ColorString str, FunctionScope* context);
  ColorString processExpression(ColorString str, FunctionScope* context, int limit);
}  // namespace omush
#endif  //
