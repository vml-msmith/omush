/**
 * \file attributetable.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_ATTRIBUTETABLE_H_
#define OMUSH_HDRS_OMUSH_ATTRIBUTETABLE_H_

namespace omush {
  struct Atrribute {
    std::string name;

    uint32_t setPermission;
    uint32_t viewPermission;
    uint32_t flags;
    Attribute()
    Attribute(std::string name) {} : special(false), name(n) {}
  };

  class AtrributeTable {
   public:
    Attribute& addAttribute(std::string name);
   private:
    std::map<std::string,Attribute*> attributeMap_;
  };


}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ATTRIBUTETABLE_H_
