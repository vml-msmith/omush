/// Copyright 2014 Michael Smith

#import "omush/game.h"
#import "omush/signalhandler.h"
#import "omush/network/network.h"
#include "omush/network/queue.h"
#import "signal.h"
#import "time.h"

#import <stdio.h>


namespace omush {

  ///
  /// Constructor. Do any initial resetting or setup of theg game that doesn't
  /// require on external configuration.
  ///
  Game::Game() : shutdown(false) {
  }


  ///
  /// Destructor. Free up any resources that were allocated during the lifetime
  /// of the game.
  ///
  Game::~Game() {
  }


  ///
  /// The main loop of the program. When this method returns, the program
  /// should end.
  ///
  void Game::run() {
    // Setup signals.
    SignalHandler::setupSignalHandling();
    SignalHandler::registerDelegate(this, SIGINT);


    // Setup network.
omush::network::InputQueue inputQueue;
    omush::network::Network server;
    server.listen(1701);
    server.setupQueues(inputQueue);

    int number_of_runs = 40;
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
        server.poll();
        ++loop;
if (inputQueue.hasMessages()) { std::cout <<  inputQueue.popMessage() <<  std::endl; }
      }
    }

    server.shutdown();
  }


  ///
  /// Handle SIGINT by setting the shutdown flag on the game.
  ///
  /// @param signum Signal caught by SignalHandler.
  ///
  void Game::handleSignal(int signum) {
    this->shutdown = true;
  }

}  // namespace omush

