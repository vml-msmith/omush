/**
 * \file lexxer.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_LEXXER_H_
#define OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_LEXXER_H_

#include <string>
#include <queue>

namespace omush {
#define TOKEN_TYPE_UP           "UP"
#define TOKEN_TYPE_DOWN         "DOWN"
#define TOKEN_TYPE_STRING       "STRING"
#define TOKEN_TYPE_SEPARATOR    "SEPARATOR"
#define TOKEN_TYPE_REPLACER     "REPLACER"
#define TOKEN_TYPE_PARCEL_IN    "PARCEL_IN:"
#define TOKEN_TYPE_PARCEL_OUT   "PARCEL_OUT"

  typedef enum {
    TokenTypeUp,
    TokenTypeDown,
    TokenTypeString,
    TokenTypeSeparator,
    TokenTypeReplacer,
    TokenTypeParcelIn,
    TokenTypeParcelOut,
  } TokenType;


  class Lexxer {
  public:
    Lexxer();
    void lex(std::string code);

    void addToken(TokenType type, char c);

    void addToken(TokenType type, std::string c);

    char retract();

    char advance();


    std::deque<std::pair<TokenType,std::string> > tree();
  private:
    bool isNumber(char s);

    bool isSeparator(char i);

    bool isDown(char i);

    bool isUp(char i);

    bool isParcelIn(char i);

    bool isParcelOut(char i);

    bool isString(char i);

    bool isReplacer(char i);

    bool isSpace(char i);


    int counter_;
    std::string codeString_;
    std::deque<std::pair<TokenType,std::string> > tree_;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_FUNCTION_ENGINE_LEXXER_H_
