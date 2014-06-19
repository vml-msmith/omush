/**
 * \file parser.h
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/function/engine/parser.h"
#include "omush/function/engine/lexxer.h"
#include <string>
#include "omush/utility.h"


namespace omush {
  Parser::~Parser() {
    removeAllNodes();
  }

  void Parser::removeAllNodes() {
    removeChildren(root);
  }

  void Parser::removeChildren(ParserNode *n) {
    while (!n->leafs.empty()) {
      removeChildren(n->leafs.front());
      n->leafs.pop();
    }

    delete n;
  }

  void Parser::insertRootNode() {
    root = new ParserNode();
    current = root;
  }

  void Parser::insertNode() {
    level += 1;
    pprint("insert");
    if (root == NULL)
      return insertRootNode();

    ParserNode *node = new ParserNode();
    node->parent = current;
    current->leafs.push(node);
    current = node;
  }

  void Parser::moveUp() {

    level -= 1;

    pprint("move up");
    if (current->parent == NULL) {
      current->parent = new ParserNode();
      current->parent->method = "concat";
      current->parent->parent = NULL;
      current->parent->leafs.push(current);
      root = current->parent;
    }
    current = current->parent;

  }

  void Parser::setCurrentMethod(std::string method) {
    pprint("method: " + method);
    current->method = method;
  }

  void Parser::addPrefix(std::string prefix) {
    current->prefix = prefix;
  }


  void  Parser::tprint(ParserNode *n, int level) {
    return;
    std::cout << "Tprint" << std::endl;
    std::string outStr = "";
    for (int i = 0; i != (level * 3); ++i) {
      outStr = outStr + " ";
    }

    outStr = outStr;
    std::cout << outStr << "Node {" << std::endl;;
    std::cout << outStr << "  Prefix = " << n->prefix << std::endl;
    std::cout << outStr << "  Method = " << n->method << std::endl;
    std::cout << outStr << "  leafs = { " << std::endl;

    int s = n->leafs.size();
    for (int i = 0; i < s; ++i) {
      tprint(n->leafs.front(), level + 1);
      n->leafs.push(n->leafs.front());
      n->leafs.pop();
    }

    std::cout << outStr << "  }" << std::endl;
    std::cout << outStr << "}" << std::endl;
  }

  void  Parser::pprint(std::string str) {
    return;
    /**/
    std::string outStr = "";
    for (int i = 0; i <= (level * 3); ++i) {
      outStr = outStr + " ";
    }
    outStr = outStr + "- " + str;
    std::cout << outStr << std::endl;
    /**/
  }


  void Parser::parse(Lexxer l) {
    insertNode();
    TokenType tokenLast;
    std::deque<std::pair<TokenType,std::string> > lex = l.tree();

    while (!lex.empty()) {
      std::pair<TokenType, std::string> part = lex.front();
      switch (part.first) {
      case TokenTypeDown:
        insertNode();
        break;
      case TokenTypeUp:
        moveUp();
        break;
      case TokenTypeSeparator:
        moveUp();
        insertNode();
        break;
      case TokenTypeParcelIn:
        //        if (tokenLast
        if (current->method != "" || !current->leafs.empty()) {
          moveUp();
          insertNode();
        }

        setCurrentMethod("concat");
        insertNode();
        break;

      case TokenTypeParcelOut:
        moveUp();
        break;

      default:
        if (current->method.compare("concat") && current->method.compare("")) {
          addPrefix(current->method);
        }

        if (current->method.compare("concat") == 0) {
          insertNode();
          setCurrentMethod(part.second);
          moveUp();
        }
        else {
          setCurrentMethod(part.second);
        }
      }

      tokenLast = part.first;
      lex.pop_front();
    }
    tprint(root, 0);
  }
}  // namespace omush
