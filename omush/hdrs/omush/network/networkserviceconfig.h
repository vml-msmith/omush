///
/// networkserviceconfig.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICECONFIG_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICECONFIG_H_

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include <boost/uuid/uuid.hpp>


namespace omush {
  namespace network {

    struct NetworkServiceConfig {
      int port;
    };

  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_NETWORKSERVICECONFIG_H_
