/**
 * \file lexxer.h
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/function/engine/lexxer.h"
#include <string>
#include "omush/utility.h"

namespace omush {

  Lexxer::Lexxer() {
  }

  void Lexxer::lex(std::string code) {
    counter_ = -1;
    codeString_ = code;

    char str = advance();
    while (str != '\0') {
      if (isUp(str)) {
        addToken(TokenTypeUp, str);
      }
      else if (isDown(str)) {
        if (tree_.back().first == TokenTypeString) {
          std::vector<std::string> parts = splitStringIntoSegments(tree_.back().second, " ", 2);

          if (parts.size() > 1) {
            tree_.pop_back();
            addToken(TokenTypeString, parts[1]);
            addToken(TokenTypeString, parts[0]);
          }
        }
        else if (tree_.back().first == TokenTypeUp ||
                 tree_.back().first == TokenTypeParcelOut) {
          // Loop back until we have the openeing.
          TokenType lookFor = TokenTypeDown;
          if (tree_.back().first == TokenTypeParcelOut)
            lookFor = TokenTypeParcelIn;

          int count = 0;
          std::deque<std::pair<TokenType,std::string> > backup;
          std::string expression = "";
          while (tree_.back().first != lookFor) {
            backup.push_back(tree_.back());
            expression = tree_.back().second + expression;
            tree_.pop_back();
          }

          backup.push_back(tree_.back());
          expression = tree_.back().second + expression;
          tree_.pop_back();

          if (lookFor == TokenTypeDown) {
            backup.push_back(tree_.back());
            expression = tree_.back().second + expression;
            tree_.pop_back();
          }

          addToken(TokenTypeString, expression);
          // Make last expression.
        }

        addToken(TokenTypeDown, str);
      }
      else if (isParcelIn(str)) {
        addToken(TokenTypeParcelIn, str);
      }
      else if (isParcelOut(str)) {
        addToken(TokenTypeParcelOut, str);
      }
      else if (isSeparator(str)) {
        addToken(TokenTypeSeparator, str);
      }
      else if (isReplacer(str)) {
        char next = advance();
        if (isReplacer(next)) {
          addToken(TokenTypeString, str);
        }
        else if (next == 'r') {
          addToken(TokenTypeString, "\n");
        }
        else {
          addToken(TokenTypeReplacer, str);
        }
      }
      // TODO: Accomodate '\'
      else {
        char next = advance();
        std::string current = std::string(1, str);
        while (isString(next)) {
          current = current + std::string(1, next);
          next = advance();
        }
        retract();
        addToken(TokenTypeString, current);
      }

      str = advance();
    }
  }

  void Lexxer::addToken(TokenType type, char c) {
    addToken(type, std::string(1, c));
  }

  void Lexxer::addToken(TokenType type, std::string c) {
    tree_.push_back(std::make_pair<TokenType&,std::string&>(type, c));
  }

  char Lexxer::retract() {
    --counter_;
    return codeString_.substr(counter_,1).c_str()[0];
  }

  char Lexxer::advance() {
    ++counter_;

    if (counter_ >= codeString_.length())
      return '\0';

    return codeString_.substr(counter_,1).c_str()[0];
  }

  std::deque<std::pair<TokenType,std::string> > Lexxer::tree() {
    return tree_;
  }

  bool Lexxer::isNumber(char s) {
    return isdigit(s);
  }

  bool Lexxer::isSeparator(char i) {
    return i == ',';
  }

  bool Lexxer::isDown(char i) {
    return i == '(';
  }

  bool Lexxer::isUp(char i) {
    return i == ')';
  }

  bool Lexxer::isParcelIn(char i) {
      return i == '[';
    }

  bool Lexxer::isParcelOut(char i) {
    return i == ']';
  }

  bool Lexxer::isString(char i) {
    return !isUp(i) && !isDown(i) && !isSeparator(i) && i != '\0' && !isParcelOut(i) && !isParcelIn(i);
  }

  bool Lexxer::isReplacer(char i) {
    return i == '%';
  }

  bool Lexxer::isSpace(char i) {
    return i == ' ';
  }
}  // namespace omush
