/**
 * \file parser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_EXECUTOR_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_EXECUTOR_H_

#include <string>
#include <queue>
#include <map>

namespace omush {
  class Parser;
  class ParserNode;
  class Function;
  class Executor {
  public:
    Executor();
    std::string execute(Parser& p);

    void linePrint(std::string str);

    void  tprint(ParserNode *n, int level);

    std::string nodeToString(ParserNode* n);

    std::string  executeNode(ParserNode* n);
    std::string strParse(std::string input);

  protected:
    std::map<std::string,Function*> funcs;
  };


}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_EXECUTOR_H_
