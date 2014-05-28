///
/// gametimer.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_GAMETIMER_H_
#define OMUSH_HDRS_OMUSH_GAMETIMER_H_

#include <vector>
#include <boost/function.hpp>

namespace omush {
  typedef boost::function<bool()> boolCallback;
  typedef std::pair<double, boolCallback> timeBoolCallbackPair;
  ///
  ///
  ///
  class GameTimer {
   public:
    GameTimer(double loopSeconds, int loopLimit);
    ~GameTimer();

    void run();
    void registerInterupt(boolCallback callback);
    void registerCallback(double limitSeconds, boolCallback callback);

   private:

    boolCallback interuptCallback_;
    std::vector<timeBoolCallbackPair> timedCallbacks_;
    double loopSeconds_;
    int loopLimit_;
    int loops_;

    GameTimer(const GameTimer&);
    void operator=(const GameTimer&);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_GAME_H_
