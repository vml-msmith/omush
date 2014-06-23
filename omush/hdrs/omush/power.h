/**
 * \file power.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_POWER_H_
#define OMUSH_HDRS_OMUSH_POWER_H_

#include <string>
#include <vector>
#include <map>

namespace omush {
  enum PowerOptions {
    PowerTo,
    PowerOverAll,
    PowerOverLowerClass,
    PowerOverAllEmpire,
    PowerOverLowerClassEmpire,
    PowerOverAllDivision,
    PowerOverLowerClassDivision,
  };

  typedef struct PowerStruct {
    std::string name;
    std::vector<std::string> alias;
    uint64_t bit;
    bool sub;

    PowerStruct() {}
    PowerStruct(std::string n) : name(n) {}
  } Power;

  class PowerDirectory {
   public:
    uint64_t add(Power p);
    uint64_t getPowerBit(std::string p);
    Power* getPower(std::string p);

   private:
    uint64_t highBit;
    std::map<uint64_t, Power*> powerBitMap;
    std::map<std::string, Power> powerMap;
  };
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_POWER_H_
