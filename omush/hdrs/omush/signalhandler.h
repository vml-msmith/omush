///
/// signalhandler.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_SIGNALHANDLER_H_
#define OMUSH_HDRS_OMUSH_SIGNALHANDLER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include <utility>

namespace omush {

  ///
  /// Delegatge interface for any object that needs to handle signals.
  ///
  class SignalHandlerDelegate {
   public:
    virtual void handleSignal(int signal) = 0;
  };

  ///
  /// Multimap holding sginal -> delegate.
  ///
  typedef std::multimap<int, SignalHandlerDelegate*> SignalDelegateMap;

  ///
  /// Pair holding singal -> delegate.
  ///
  typedef std::pair<int, SignalHandlerDelegate*> SignalDelegatePair;

  ///
  /// Static class that handles catching signals.
  ///
  class SignalHandler {
   public:
    static void registerDelegate(SignalHandlerDelegate*, int);
    static void setupSignalHandling();
    static void signalHandler(int signum);

   private:
    static SignalDelegateMap delegates_;
    SignalHandler(const SignalHandler&);
    void operator=(const SignalHandler&);
    SignalHandler() {}
    ~SignalHandler() {}
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_SIGNALHANDLER_H_
