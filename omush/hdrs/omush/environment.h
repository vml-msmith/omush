///
/// environment.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_ENVIRONMENT_H_
#define OMUSH_HDRS_OMUSH_ENVIRONMENT_H_

namespace omush {
  namespace Network {
    class Server;
  }
  namespace database {
    class Database;
  }

  struct Environment {
    database::Database *database;
    Network::Server *server;
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_ENVIRONMENT_H_
