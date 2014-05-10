///
/// game.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_GAME_H_
#define OMUSH_HDRS_OMUSH_GAME_H_

#import "signalhandler.h"

namespace omush {

  ///
  /// This is the main class that is the the game. Everything is handled from
  /// here.
  ///
  class Game : public SignalHandlerDelegate {
   public:
    Game();
    ~Game();
    void run();

    virtual void handleSignal(int signal);

   private:
    bool shutdown;

    Game(const Game&);
    void operator=(const Game&);
  };

}  // namespace omush

#endif  // OMUSH_HDRS_OMUSH_GAME_H_
