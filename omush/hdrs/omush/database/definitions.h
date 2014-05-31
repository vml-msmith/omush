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
      DbObjectTypeExit,
    };
    struct DatabaseAttribute;
    class DatabaseObject;
    class DatabaseObjectFactory;
    class PlayerUtilities;
    class Database;
  }  // namespace database
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_DATABASE_DEFINITIONS_H_
