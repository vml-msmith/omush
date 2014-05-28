/**
 * \file networkmessage.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_NETWORK_NETWORKMESSAGE_H_
#define OMUSH_HDRS_OMUSH_NETWORK_NETWORKMESSAGE_H_

#include <string>
#include <boost/uuid/uuid.hpp>

namespace omush {
  namespace network {
    /**
     * Unique ID for a connected descriptor.
     */
    typedef boost::uuids::uuid ConnectionId;

    struct NetworkMessage {
      ConnectionId id;
      std::string rawString;
    };
  }  // namespace network
}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_NETWORK_NETWORKMESSAGE_H_
