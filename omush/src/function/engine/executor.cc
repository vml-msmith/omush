/**
 *
 *
 */

#include "omush/function/engine/executor.h"
#include "omush/function/engine/parser.h"
#include "omush/function/function.h"
#include "omush/function/engine/lexxer.h"
namespace omush {
  Executor::Executor() {
    /*
    funcs["add"] = new FunctionAdd();
    funcs["sub"] = new FunctionSub();
    funcs["mul"] = new FunctionMul();
    funcs["if"] = new FunctionIf();
    */
  }

  std::string Executor::execute(Parser& p) {
    ParserNode *n = p.root;

    return executeNode(n);
  }


  void Executor::linePrint(std::string str) {
    std::cout << "# " << str << std::endl;
  }

  void  Executor::tprint(ParserNode *n, int level) {
    //    return;
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

  std::string Executor::nodeToString(ParserNode* n) {
    int s = n->leafs.size();

    std::string inners = "";
    for (int i = 0; i < s; ++i) {

      inners += "," + nodeToString(n->leafs.front());
      n->leafs.push(n->leafs.front());
      n->leafs.pop();
    }

    std::string response = n->method;
    if (s > 0) {
      inners = inners.substr(1,inners.length());
      response = "[" + response + "(" + inners + ")]";
    }

    return response;
  }

  std::string  Executor::executeNode(ParserNode* n) {
    std::string response;
    std::vector<std::string> responses;
    bool hasLeaves = false;
    int numberOfLeaves = n->leafs.size();

    n->prefix = strParse(n->prefix);
    if (n->method.compare("concat"))
      n->method = strParse(n->method);


    Function* fun = NULL;
    std::string method = n->method;
    boost::trim(method);
    if (funcs.find(method) != funcs.end()) {
      fun = funcs[method];
    }

    for (int i = 0; i < numberOfLeaves; ++i ) {
      hasLeaves = true;
      //std::string ex = n->leafs.front();

      if (fun == NULL || fun->shouldEvaluateParameter(i)) {
        responses.push_back(executeNode(n->leafs.front()));
      }
      else {
        responses.push_back(nodeToString(n->leafs.front()));
      }
      //      std::string ex;
      //      responses.push_back(executeNode(n->leafs.front()));
      n->leafs.push(n->leafs.front());
      n->leafs.pop();
    }

    if (numberOfLeaves == 0 || method.compare("concat") == 0) {
      if (n->method.compare("concat")) {
        response += n->method;
      }
      for(std::vector<std::string>::iterator it = responses.begin();
          it != responses.end();
          ++it) {
        response += *it;
      }
    }

    else if (funcs.find(method) == funcs.end()) {
      response = "#-1 FUNCTION (" + method + ") NOT FOUND";
    } else {
      std::string out = method + "(";
      for(std::vector<std::string>::iterator iter = responses.begin();
          iter != responses.end();
          ++iter) {
        out += *iter;
        out += ", ";
      }
       boost::trim_if(out, boost::is_any_of(", "));
      out += ") => ";
      std::string res = funcs[method]->run(responses);
      out += res;
      linePrint(out);
      response += res;
    }
    // TODO(Delete here)

    return n->prefix + response;
  }

  std::string Executor::strParse(std::string input) {
    Lexxer l = Lexxer();
    l.lex(input);

    std::deque<std::pair<TokenType,std::string> > tree = l.tree();

    Parser p = Parser();
    p.parse(l);
    if (l.tree().size() > 1) {
      Executor e = Executor();
      return e.execute(p);
    }
    return input;

    //parse = Parser().parse(lex);


    return input;
  }

}  // namespace omush
