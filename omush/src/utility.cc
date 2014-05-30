#include "omush/utility.h"
#include <map>
#include <vector>

namespace omush {
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
