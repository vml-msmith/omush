/**
 * \file parser.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_PARSER_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_PARSER_H_

#include <string>
#include <queue>

namespace omush {
  class Lexxer;
  struct ParserNode;
  struct ParserNode {
    ParserNode* parent;
    std::string method;
    std::string prefix;
    std::queue<ParserNode*> leafs;
    std::string result;
    std::string signature;

  ParserNode() : parent(NULL), method(""), result(""), prefix("") { }
  };

  class Parser {
   public:
    ParserNode* root;
    ParserNode* current;
    int level = 0;

    ~Parser();

    void removeAllNodes();

    void removeChildren(ParserNode *n);

    void insertRootNode();

    void insertNode();

    void moveUp();
    void setCurrentMethod(std::string method);

    void addPrefix(std::string prefix);


    void  tprint(ParserNode *n, int level);

    void  pprint(std::string str);

    void parse(Lexxer l);

  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_PARSER_H_
