/**
 * \file attributetable.cc
 *
 * Copyright 2014 Michael Smith
 */

#include "omush/database/attributetable.h"
#include <boost/algorithm/string.hpp>

namespace omush {
  namespace database {
    AttributeTable::AttributeTable() {
    }

    AttributeTable::~AttributeTable() {
      deleteAllAttributes();
    }

    AttributeTable::Attribute*
    AttributeTable::addAttribute(AttributeName name) {
      if (!hasAttributeByName(name)) {
        addAttributeByName(name);
      }

      AttributeTable::Attribute* attribute = getAttributeByName(name);
      return attribute;
    }

    bool AttributeTable::hasAttributeByName(AttributeName name) {
      boost::to_upper(name);
      if (attributeMap_.find(name) == attributeMap_.end())
        return false;

      return true;
    }

    AttributeTable::Attribute*
    AttributeTable::getAttributeByName(AttributeName name) {
      boost::to_upper(name);
      if (!hasAttributeByName(name)) {
        return NULL;
      }

      return attributeMap_[name];
    }

    void AttributeTable::deleteAllAttributes() {
      for (AttributeMap::iterator i = attributeMap_.begin();
          i != attributeMap_.end();
          ++i) {
        delete i->second;
        attributeMap_.erase(i);
      }
    }

    void AttributeTable::addAttributeByName(AttributeName name) {
      boost::to_upper(name);
      attributeMap_[name] = new Attribute(name);
    }

  }  // namespace database
}  // namespace omush
