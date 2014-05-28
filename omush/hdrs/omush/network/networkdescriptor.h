/**
 * \file networkdescriptor.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORKDESCRIPTOR_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORKDESCRIPTOR_H_

#include <boost/uuid/uuid.hpp>
#include "websocketpp/server.hpp"

namespace omush {
  namespace network {
    struct NetworkDescriptor {
      websocketpp::connection_hdl hdl;
      boost::uuids::uuid uniqueId;
    };
  }
}


#endif  // OMUSH_HDRS_OMUSH_COMMAND_H_
