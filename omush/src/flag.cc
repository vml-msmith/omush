/**
 * \file flag.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/flag.h"
#include <string>
#include <vector>
#include <map>

namespace omush {
  uint64_t FlagDirectory::addFlag(Flag f) {
    if (flagMap.find(f.name) == flagMap.end()) {
      flagMap.insert(std::pair<std::string,Flag>(f.name, f));
      highBit = highBit << 1;
      f.bit = highBit;
      flagBitMap.insert(std::pair<uint64_t, Flag*>(highBit, &(flagMap[f.name])));
    }

    return flagMap[f.name].bit;
  }

  uint64_t FlagDirectory::getFlagBit(std::string f) {
    if (flagMap.find(f) == flagMap.end()) {
      return 0;
    }

    return flagMap[f].bit;
  }

  Flag* FlagDirectory::getFlag(std::string f) {
    if (flagMap.find(f) == flagMap.end()) {
      return NULL;
    }

    return &(flagMap[f]);
  }
}  // namespace omush
