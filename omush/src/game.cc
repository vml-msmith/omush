// Copyright 2014 Michael Smith

#import "omush/game.h"
#import "omush/signalhandler.h"
#import "signal.h"
#import "time.h"

#import <stdio.h>
namespace omush {

  Game::Game() : shutdown(false) {
  }

  Game::~Game() {
  }

  void Game::run() {
    // Setup signals.
    SignalHandler::setupSignalHandling();
    SignalHandler::registerDelegate(this, SIGINT);
    // Do stuff.


    int number_of_runs = 20;
    double interval = 0.5f;
    double clocks_per_ms = CLOCKS_PER_SEC / 1;
    double clock_threshhold = clocks_per_ms * interval;

    double time_counter = 0;
    clock_t this_time = clock();
    clock_t last_time = this_time;
    int loop = 0;

    while (loop < number_of_runs && !this->shutdown) {
      this_time = clock();

      time_counter += static_cast<double>(this_time - last_time);
      last_time = this_time;

      if (time_counter >= clock_threshhold) {
        time_counter = 0;
        printf("Loop: %d\n", loop);

        ++loop;
      }
    }
  }

  void Game::handleSignal(int signum) {
    this->shutdown = true;
  }

}  // namespace omush

