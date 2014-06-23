/**
 * \file power.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/power.h"
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

namespace omush {
  uint64_t PowerDirectory::add(Power p) {
    std::string name = p.name;
    boost::to_upper(name);

    if (powerMap.find(name) == powerMap.end()) {
      powerMap.insert(std::pair<std::string, Power>(name, p));
      highBit = highBit << 1;
      p.bit = highBit;
      powerBitMap.insert(std::pair<uint64_t, Power*>(highBit, &(powerMap[p.name])));
    }

    return powerMap[name].bit;
  }

  uint64_t PowerDirectory::getPowerBit(std::string p) {
    std::string name = p;
    boost::to_upper(name);

    if (powerMap.find(name) == powerMap.end()) {
      return 0;
    }

    return powerMap[name].bit;
  }

  Power* PowerDirectory::getPower(std::string p) {
    std::string name = p;
    boost::to_upper(name);

    if (powerMap.find(name) == powerMap.end()) {
      return NULL;
    }

    return &(powerMap[name]);
  }
}  // namespace omush
