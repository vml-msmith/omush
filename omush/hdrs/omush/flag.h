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
  typedef uint64_t FlagBit;

  struct Flag {
    std::string name;
    std::vector<std::string> alias;
    char letter;
    FlagBit bit;

    Flag() {}
    Flag(std::string n, char a) : name(n), letter(a) {}
  };

  class FlagDirectory {
   public:
    typedef std::map<std::string, Flag> FlagMap;
    typedef std::map<FlagBit, Flag> BitFlagMap;
    typedef std::map<FlagBit, Flag*> BitFlagPtrMap;

    FlagBit addFlag(Flag f);
    FlagBit getFlagBit(std::string f);
    Flag* getFlag(std::string f);
    FlagMap getAllFlags();

   private:
    FlagBit highBit;
    BitFlagPtrMap flagBitMap;
    FlagMap flagMap;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_FLAG_H_
