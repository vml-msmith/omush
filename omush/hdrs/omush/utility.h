/**
 * \file utility.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_UTILITY_H_
#define OMUSH_HDRS_OMUSH_UTILITY_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include<boost/tokenizer.hpp>
#include <queue>
#include <vector>

namespace omush {
std::queue<std::string> encodeString(std::string message);
 std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep, int segments);
  class ColorSequence {
  public:
    ColorSequence(std::string s, std::size_t iter) {
      sequence = s;
      start = iter;
      end = std::string::npos;
    }

    bool hasEnd() {
      if (end == std::string::npos) {
        return false;
      }
      return true;
    }

    void setEnd(std::size_t iter) {
      end = iter;
    }

    std::string buildTag() {
      std::vector<std::string> sequence_parts;
      boost::split(sequence_parts, sequence, boost::is_any_of(":"));
      std::vector<std::string> tag;
      tag.push_back("<span");
      std::string f = sequence_parts[0];
      if (f.compare("class") == 0 &&
          sequence_parts.size() > 1) {
        tag.push_back(" class=\"");
        tag.push_back(sequence_parts[1]);
      }
      else {
        tag.push_back(" style=\"");
        tag.push_back(sequence);
      }

      tag.push_back("\">");

      return boost::algorithm::join(tag, "");
    }


    std::string buildEndTag() {
      return "</span>";
    }

    std::string sequence;
    std::size_t start;
    std::size_t end;
  };

  class ColorString {
  public:

    static std::string wrap(std::string str, std::string sequence) {
      return "x1b[" + sequence + "]" +  str + "x1b[end]";
    }

    static std::string color(std::string str, std::string color) {
      color[0] = std::toupper(color[0]);
      color = "ansi" + color;
      return ColorString::wrap(str, "class:" + color);
    }

    ColorString(std::string const& s) : string_(s) {
      std::deque<ColorSequence> sequences;
      std::string myString = s;
      while (true) {
        std::size_t start = myString.find("x1b[");

        if (start == std::string::npos)
          break;

        std::size_t end = myString.find("]");

        std::string sequence = myString.substr(start + 4, end - start - 4);
        if (sequence.compare("end") == 0) {
          std::deque<ColorSequence> sQ;
          ColorSequence last("",std::string::npos);
          while (true) {
            last = sequences.back();
            sequences.pop_back();
            sQ.push_back(last);
            if (!last.hasEnd()) {
              break;
            }
          }
          sQ.back().setEnd(start);
          while (!sQ.empty()) {
            ColorSequence next = sQ.back();
            sequences.push_back(next);
            sQ.pop_back();
          }
        } else {
          sequences.push_back(ColorSequence(sequence, start));
        }
        myString = std::string(myString.substr(0, start) +
                               myString.substr(end + 1, myString.size()));
      }
      sequences_ = sequences;
      parsed_ = myString;
    }


    std::string outString() {
      std::size_t offset = 0;
      std::vector<std::string> str;
      for (size_t iter = 0; iter != parsed_.size(); ++iter) {
        str.push_back(parsed_.substr(iter,1));
      }
      while (!sequences_.empty()) {
        ColorSequence s = sequences_.back();
        sequences_.pop_back();

        str[s.start] = s.buildTag() + str[s.start];
        str[s.end - 1] = str[s.end - 1] + s.buildEndTag();
      }
      std::string out = "";

      return boost::algorithm::join(str, "");

    }

  private:
    std::string string_;
    std::string parsed_;
    std::deque<ColorSequence> sequences_;
  };
}

#endif
