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
  PowerDirectory::PowerDirectory() {
    highBit = 1;
  }
  uint64_t PowerDirectory::add(Power p) {
    std::string name = p.name;
    boost::to_upper(name);

    if (powerMap.find(name) == powerMap.end()) {
      highBit = highBit << 1;

      p.bit = highBit;

      powerMap.insert(std::pair<std::string, Power>(name, p));
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

  std::map<std::string, Power> PowerDirectory::getAllPowers() {
    return powerMap;
  }

}  // namespace omush
