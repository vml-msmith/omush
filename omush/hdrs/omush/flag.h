/**
 * \file flag.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_FLAG_H_
#define OMUSH_HDRS_OMUSH_FLAG_H_

#include <string>
#include <vector>
#include <map>

namespace omush {
  struct Flag {
    std::string name;
    std::vector<std::string> alias;
    char letter;
    uint64_t bit;
    Flag() {};
    Flag(std::string n, char a) : name(n), letter(a) {}
  };

  class FlagDirectory {
   public:
    uint64_t addFlag(Flag f);
    uint64_t getFlagBit(std::string f);
    Flag* getFlag(std::string f);
    std::map<std::string, Flag> getAllFlags();

   private:
    uint64_t highBit;
    std::map<uint64_t, Flag*> flagBitMap;
    std::map<std::string, Flag> flagMap;
  };
}  // namespace omush

#endif  //  OMUSH_HDRS_OMUSH_FLAG_H_
