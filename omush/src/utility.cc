#include "omush/utility.h"
#include <map>
#include <vector>

namespace omush {

  std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep) {
    return  splitStringIntoSegments(str, sep, -1);
  }

  std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep, int segments) {
    return splitStringIntoSegments(str, sep, segments, leftToRight);
  }

  std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep, int segments, SplitStringOptions direction) {
    if (segments < 0)
      segments = 1000;

    std::vector<std::string> result;
    int i = 0;
    std::size_t pos;
    if (direction == leftToRight) {
      pos = str.find(sep);
    }
    else {
      pos = str.rfind(sep);
    }

    while (i < (segments - 1) && pos != std::string::npos) {
      result.push_back(str.substr(0,pos));
      str = str.substr(pos + 1, str.length() - pos + 1);

      if (direction == leftToRight) {
        pos = str.find(sep);
      }
      else {
        pos = str.rfind(sep);
      }

      ++i;
    }

    result.push_back(str);
    /*
    if (direction == rightToLeft) {
      std::vector<std::string> newResult;
      for (int i = result.size() - 1; i >= 0; --i) {
        newResult.push_back(result[i]);
      }
      return newResult;
      }*/
    return result;
  }

  std::queue<std::string> encodeString(std::string message) {
    std::queue<std::string> responseQueue;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\n");
    tokenizer tokens(message, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
         tok_iter != tokens.end(); ++tok_iter) {

      std::string m = *tok_iter;

      // Characters to be transformed.
      std::map<char, std::string> transformations;
      transformations['&']  = std::string("&amp;");
      transformations['\''] = std::string("&apos;");
      transformations['"']  = std::string("&quot;");
      transformations['>']  = std::string("&gt;");
      transformations['<']  = std::string("&lt;");
      transformations[' ']  = std::string("&nbsp;");

      // Build list of characters to be searched for.
      std::string reserved_chars;
      for (auto ti = transformations.begin(); ti != transformations.end(); ++ti) {
        reserved_chars += ti->first;
      }

      size_t pos = 0;
      while (std::string::npos != (pos = m.find_first_of(reserved_chars, pos))) {
        m.replace(pos, 1, transformations[m[pos]]);
        ++pos;
      }

      responseQueue.push(m);
    }
    return responseQueue;
  }
}
