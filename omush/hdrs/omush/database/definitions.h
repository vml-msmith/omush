/**
 * \file definitions.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_DATABASE_DEFINITIONS_H_
#define OMUSH_HDRS_OMUSH_DATABASE_DEFINITIONS_H_


namespace omush {
  namespace database {
    typedef long Dbref;
    enum DbObjectType {
      DbObjectTypePlayer,
      DbObjectTypeThing,
      DbObjectTypeRoom,
    };
    struct DatabaseAttribute;
    class DatabaseObject;
    class DatabaseObjectFactory;
  }
}

#endif
