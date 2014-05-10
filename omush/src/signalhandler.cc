// Copyright 2014 Michael Smith

#include "omush/signalhandler.h"
#include <signal.h>

namespace omush {
  /// Create the delegates_ static variable.
  SignalDelegateMap SignalHandler::delegates_;

  ///
  /// Register an instance of SignalHandlerDelegate to receive siginals.
  ///
  /// @param delegate   Instance of SiginalHandlerDelegate that will receive the
  ///                   signal.
  /// @param signal     The signal that the delegate will respond to.
  ///
  void SignalHandler::registerDelegate(SignalHandlerDelegate* delegate,
                                       int signal) {
    delegates_.insert(SignalDelegatePair(signal, delegate));
  }


  ///
  /// Method to intercept singals. Registered by setupSiginalHandling().
  ///
  /// Will call handleSignal() on any SignalHandlerDelegate that has registered
  /// with signum. Signum is passed to handleSignal() so the
  /// SignalHandlerDelegate knows how to handle it.
  ///
  /// @param signum The siginal caught by the class.
  ///
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


  ///
  /// Register any siginals we care about in the application. Those siginals
  /// will be caught by signalHandler().
  ///
  void SignalHandler::setupSignalHandling() {
    signal(SIGINT, SignalHandler::signalHandler);
  }
}  // namespace omush
