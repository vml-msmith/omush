/// Copyright 2014 Michael Smith

#include "omush/gametimer.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

namespace omush {

  GameTimer::GameTimer(double loopSeconds, int loopLimit )
    : loopSeconds_(loopSeconds), loopLimit_(loopLimit) {
      interuptCallback_ = NULL;
  }


  GameTimer::~GameTimer() {
  }


  void GameTimer::run() {
    if (interuptCallback_ == NULL)
      return;

    double clock_threshhold = CLOCKS_PER_SEC * loopSeconds_;
    clock_t this_time = clock();
    clock_t last_time = this_time;

    int loops = 0;
    while (loopLimit_ == 0 || loops <= loopLimit_) {
      if (interuptCallback_())
        break;

      this_time = clock();

      std::vector<timeBoolCallbackPair>::iterator iter;

      for (iter = timedCallbacks_.begin(); iter != timedCallbacks_.end(); ++iter) {
        if ((*iter).second == NULL)
          continue;

        double methodThreshhold = (*iter).first * CLOCKS_PER_SEC;
        double methodTime = this_time;
        while (methodTime - this_time < methodThreshhold) {
          if (!(*iter).second()) {
            break;
          }
          methodTime = clock();
        }
      }

      if (interuptCallback_())
        break;

      // Reach the end of what we need done. If there is any time left, sleep.
      if (this_time - last_time < clock_threshhold) {
        boost::this_thread::sleep(boost::posix_time::microseconds(clock_threshhold - (this_time - last_time)));
      }
      last_time = this_time;
      ++loops;
    }
  }

  void GameTimer::registerInterupt(boolCallback callback) {
    interuptCallback_ = callback;
  }

  void GameTimer::registerCallback(double limitSeconds, boolCallback callback)  {
    timedCallbacks_.push_back(std::make_pair(limitSeconds, callback));
  }

}
