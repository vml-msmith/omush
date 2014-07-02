/**
 * \file actionsetattribute.cc
 *
 * Copyright 2014 Michael Smith
 */


#include "omush/action/actionsetattribute.h"

namespace omush {
  ActionSetAttribute::ActionSetAttribute(CommandContext& context)
    : context_(context), target_(NULL), attribute_(""), value_("") {
  }

  ActionSetAttribute& ActionSetAttribute::target(database::DatabaseObject* object) {
    target_ = object;
    return *this;
  }
  ActionSetAttribute& ActionSetAttribute::attribute(std::string attribute) {
    attribute_ = attribute;
    return *this;
  }
  ActionSetAttribute& ActionSetAttribute::value(std::string value) {
    value_ = value;
    return *this;
  }


  void ActionSetAttribute::enact() {
    std::cout << target_->getAttribute("name").value << std::endl;
    std::cout << "Attr: " << attribute_ << std::endl;
    std::cout << "Value: " << value_ << std::endl;

    target_->setAttribute(attribute_, value_);
    std::string message = attribute_ + " set.";
    Notifier(*(context_.game),
             *(context_.db)).notify(context_.cmdScope.executor,
                                    message);
  }
}  // namespace omush
