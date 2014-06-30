/**
 * \file attributetable.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_ATTRIBUTETABLE_H_
#define OMUSH_HDRS_OMUSH_DATABASE_ATTRIBUTETABLE_H_

#include <map>
#include <string>

namespace omush {
  namespace database {

    class AttributeTable {
     public:
      typedef std::string AttributeName;
      struct Attribute {
        uint32_t setPermission;
        uint32_t viewPermission;
        uint32_t flags;
        Attribute() {}
        explicit Attribute(AttributeName n) :
        name_(n), setPermission(0), viewPermission(0), flags(0) {}

       private:
        AttributeName name_;
        AttributeName name() { return name_; }
      };
      typedef std::map<AttributeName, Attribute*> AttributeMap;

     public:
      AttributeTable();
      ~AttributeTable();
      Attribute* addAttribute(AttributeName name);
      Attribute* getAttributeByName(AttributeName name);
      bool hasAttributeByName(AttributeName name);

     private:
      void addAttributeByName(AttributeName name);
      void deleteAllAttributes();

      AttributeMap attributeMap_;
    };
  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_ATTRIBUTETABLE_H_
