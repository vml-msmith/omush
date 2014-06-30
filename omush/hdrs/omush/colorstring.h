/**
 * \file utility.h
 *
 * Copyright 2014 Michael Smith
 *
 * The ColorString class represents an intermal MUSH string that has been
 * sent throug the expression engine or otherwise includes meta-data about how
 * it should be displayed.
 *
 * Just adding tags to strings for color, underlines or other options will
 * create strings that we can't utilize MUHS softcode operations on. GEtting
 * the string length of a string should not return the length of the string
 * with all the extra information for instance, it should only return the number
 * of characters in the visible string.
 *
 * Most (if not all) MUSH functions dealing with extracting data or manipulating
 * a string should have a counterpart method on the ColorString class.
 *
 * Ex:
 *  strlen(<span color="red">Hi</span>) should only return "2".
 */

#ifndef OMUSH_HDRS_OMUSH_COLORSTRING_H_
#define OMUSH_HDRS_OMUSH_COLORSTRING_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include<boost/tokenizer.hpp>
#include <queue>
#include <vector>

namespace omush {

  class ColorSequence {
   public:
    ColorSequence();
    ColorSequence(std::string s, std::size_t iter);

    bool hasEnd();

    void setEnd(std::size_t iter);

    std::string buildTag();

    std::string buildEndTag();

    std::string buildInternalTag();

    std::string buildInternalEndTag();


    std::string sequence;
    std::size_t start;
    std::size_t end;
  };

  struct ColorStringNode {
    char c;
    bool isSequence;
    bool isEnd;
    ColorSequence seq;

    ColorStringNode(char c) : c(c), isSequence(false), isEnd(false) {  }
    ColorStringNode(ColorSequence c) : seq(c), isSequence(true), isEnd(false) {  }
    ColorStringNode() :  isSequence(false) {  }
  };

  class ColorString {
   public:
    std::deque<ColorStringNode*> nodes;

    ColorString(const ColorString& other);
    ColorString& operator=(const ColorString& other);
    void reset();
    std::deque<ColorStringNode*> makeNodeCopy() const;

    ~ColorString();

    ColorString(std::string s);

    std::string outString();

    std::string internalString();

    void trim_left(std::string sequence);

    void trim_right(std::string sequence);

    std::string basicString();

    static std::string wrap(std::string str, std::string sequence);

    static std::string color(std::string str, std::string color);

    void to_upper();
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_COLORSTRING_H_
