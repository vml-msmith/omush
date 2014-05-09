// Copyright 2014 Michael Smith

#include "omush/signalhandler.h"
#include <signal.h>

namespace omush {
  SignalDelegateMap SignalHandler::delegates_;
  void SignalHandler::registerDelegate(SignalHandlerDelegate* delegate,
                                       int signal) {
    delegates_.insert(SignalDelegatePair(signal, delegate));
  }

  void SignalHandler::signalHandler(int signum) {
    printf("Signal");
    std::pair<SignalDelegateMap::iterator, SignalDelegateMap::iterator> ret;
    ret = delegates_.equal_range(signum);

    for (SignalDelegateMap::iterator it = ret.first;
         it != ret.second;
         ++it) {
      printf("Delegates..");
      (it->second)->handleSignal(signum);
    }
  }

  void SignalHandler::setupSignalHandling() {
    signal(SIGINT, SignalHandler::signalHandler);
  }
}  // namespace omush
