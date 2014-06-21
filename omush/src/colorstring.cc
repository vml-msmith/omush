/**
 * \file utility.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/colorstring.h"

namespace omush {
  ColorSequence::ColorSequence() {}
  ColorSequence::ColorSequence(std::string s, std::size_t iter) {
    sequence = s;
    start = iter;
    end = std::string::npos;
  }
  bool ColorSequence::hasEnd() {
    if (end == std::string::npos) {
      return false;
    }
    return true;
  }

  void ColorSequence::setEnd(std::size_t iter) {
    end = iter;
  }

  std::string ColorSequence::buildTag() {
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

  std::string ColorSequence::buildEndTag() {
    return "</span>";
  }

  std::string ColorSequence::buildInternalTag() {
    std::vector<std::string> sequence_parts;

    std::vector<std::string> tag;

    tag.push_back("x1b|");
    tag.push_back(sequence);
    tag.push_back("|");

    return boost::algorithm::join(tag, "");
  }


  std::string ColorSequence::buildInternalEndTag() {
    return "x1b|end|";
  }

  ColorString::ColorString(const ColorString& other) {
    //    nodes = other.makeNodeCopy();
    reset();
    for (std::deque<ColorStringNode*>::const_iterator i = other.nodes.begin();
         i != other.nodes.end();
         ++i) {
      nodes.push_back(new ColorStringNode());
      nodes.back()->c = (*i)->c;
      nodes.back()->isSequence = (*i)->isSequence;
      nodes.back()->isEnd = (*i)->isEnd;
      nodes.back()->seq = (*i)->seq;
    }
  }

  ColorString& ColorString::operator=(const ColorString& other) {
    //    nodes = other.makeNodeCopy();
    reset();
    for (std::deque<ColorStringNode*>::const_iterator i = other.nodes.begin();
         i != other.nodes.end();
         ++i) {
      nodes.push_back(new ColorStringNode());
      nodes.back()->c = (*i)->c;
      nodes.back()->isSequence = (*i)->isSequence;
      nodes.back()->isEnd = (*i)->isEnd;
      nodes.back()->seq = (*i)->seq;
    }

    return *this;
  }

  void ColorString::reset() {
    while (!nodes.empty()) {
      delete nodes.front();
      nodes.pop_front();
    }
  }

  ColorString::~ColorString() {
    reset();
  }

  ColorString::ColorString(std::string s) {
    std::deque<ColorSequence> sequences;
    std::string myString = s;
    while (true) {
      std::size_t start = myString.find("x1b|");

      if (start == std::string::npos)
        break;

      std::size_t end = myString.find("|", start + 4);
      for (int i = 0; i < start; ++i) {
        ColorStringNode* n = new ColorStringNode(myString.c_str()[i]);
        nodes.push_back(n);
      }

      std::string sequence = myString.substr(start + 4, end - start - 4);

      if (sequence.compare("end") == 0) {
        std::deque<ColorStringNode*> sQ;
        ColorStringNode *last;

        while (true) {
          last = nodes.back();
          nodes.pop_back();
          sQ.push_back(last);
          if (last->isSequence && !last->seq.hasEnd()) {
            break;
          }
        }
        sQ.back()->seq.setEnd(start);
        while (!sQ.empty()) {
          nodes.push_back(sQ.back());
          sQ.pop_back();
        }

        nodes.push_back(new ColorStringNode(ColorSequence(sequence, start)));
        nodes.back()->isEnd = true;
      } else {
        nodes.push_back(new ColorStringNode(ColorSequence(sequence, start)));
      }

      myString = std::string(myString.substr(end + 1, myString.size()));
    }

    for (int i = 0; i < myString.length(); ++i) {
      ColorStringNode* n = new ColorStringNode(myString.c_str()[i]);
      nodes.push_back(n);
    }

  }

  std::string ColorString::outString() {
    std::string str;
    for (int i = 0; i < nodes.size(); ++i) {

      if (nodes.front()->isEnd) {
        str += nodes.front()->seq.buildEndTag();
      }
      else if (nodes.front()->isSequence) {
        str += nodes.front()->seq.buildTag();
      }
      else
        str += nodes.front()->c;


      nodes.push_back(nodes.front());
      nodes.pop_front();
    }
    return str;
  }

  std::string ColorString::internalString() {
    std::string str;

    for (int i = 0; i < nodes.size(); ++i) {
      if (nodes.front()->isEnd) {
        str += nodes.front()->seq.buildInternalEndTag();
      }
      else if (nodes.front()->isSequence) {
        str += nodes.front()->seq.buildInternalTag();
      }
      else {
        str += nodes.front()->c;
      }


      nodes.push_back(nodes.front());
      nodes.pop_front();
    }
    return str;
  }


  void ColorString::trim_left(std::string sequence) {
    std::deque<ColorStringNode*> replace;
    std::deque<ColorStringNode*> hold;

    char next = sequence.c_str()[0];
    int count = 0;

    while (!nodes.empty()) {
      if (nodes.front()->isSequence || nodes.front()->isEnd) {
        replace.push_back(nodes.front());
        nodes.pop_front();
        continue;
      }

      if (nodes.front()->c == next) {
        hold.push_back(nodes.front());
        nodes.pop_front();
        ++count;
        if (count == sequence.length()) {
          // Delete them forever!
          while (!hold.empty()) {
            delete hold.front();
            hold.pop_front();
          }
          count = 0;
        }
        next = sequence.c_str()[count];
      }
      else {
        break;
      }
    }

    while (!hold.empty()) {
      nodes.push_front(hold.back());
      hold.pop_back();
    }

    while (!replace.empty()) {
      nodes.push_front(replace.back());
      replace.pop_back();
    }
  }

  void ColorString::trim_right(std::string sequence) {
    std::deque<ColorStringNode*> replace;
    std::deque<ColorStringNode*> hold;

    char next = sequence.c_str()[0];
    int count = 0;

    while (!nodes.empty()) {
      if (nodes.back()->isSequence || nodes.back()->isEnd) {
        replace.push_back(nodes.back());
        nodes.pop_back();
        continue;
      }

      if (nodes.back()->c == next) {
        hold.push_back(nodes.back());
        nodes.pop_back();
        ++count;
        if (count == sequence.length()) {
          // Delete them forever!
          while (!hold.empty()) {
            delete hold.front();
            hold.pop_front();
          }
          count = 0;
        }
        next = sequence.c_str()[count];
      }
      else {
        break;
      }
    }

    while (!hold.empty()) {
      nodes.push_back(hold.back());
      hold.pop_back();
    }

    while (!replace.empty()) {
      nodes.push_back(replace.back());
      replace.pop_back();
    }
  }

  std::string ColorString::basicString() {
    std::string str = "";

    for (int i = 0; i < nodes.size(); ++i) {

      if (!nodes.front()->isSequence && !nodes.front()->isEnd) {
        str += nodes.front()->c;
      }

      nodes.push_back(nodes.front());
      nodes.pop_front();
    }
    std::cout << std::endl;

    return str;
  }


  std::string ColorString::wrap(std::string str, std::string sequence) {
    return "x1b|" + sequence + "|" +  str + "x1b|end|";
  }

  std::string ColorString::color(std::string str, std::string color) {
    color[0] = std::toupper(color[0]);
    color = "ansi" + color;
    return ColorString::wrap(str, "class:" + color);
  }

  void ColorString::to_upper() {
    for (int i = 0; i < nodes.size(); ++i) {

      //char c = nodes.front()->c;
      toupper(nodes.front()->c);

      //      boost::to_upper(nodes.front()->c);
      nodes.push_back(nodes.front());
      nodes.pop_front();
    }
  }

}  // namespace omush
