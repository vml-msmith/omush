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
  enum SplitStringOptions {
    leftToRight,
    rightToLeft,
  };
  std::queue<std::string> encodeString(std::string message);


  std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep);
  std::vector<std::string> splitStringIntoSegments(std::string str, std::string sep, int segments);

  std::vector<std::string> splitStringIntoSegments(std::string str,
                                                   std::string sep,
                                                   int segments,
                                                   omush::SplitStringOptions direction);


}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_UTILITY_H_
