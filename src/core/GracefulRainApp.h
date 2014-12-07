/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef GRACEFULRAIN_GRACEFULRAINAPP_H_
#define GRACEFULRAIN_GRACEFULRAINAPP_H_

#include "core/GracefulRainGame.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/misc/FPSCounter.h"

class GracefulRainApp {
 public:
  GracefulRainApp();
  ~GracefulRainApp();

  int Run();

 private:
  static const std::string kWindowCaption;
  static const glm::u32vec2 kWindowPosition;
  static const Uint32 kWindowScale;
  static const Uint32 kIdealFrameRate;
  static const int kFPSCountSamplingTime;
  static const int kTweakBarWidth;
  static const unsigned char kTweakBarColor[3];

  void CleanUp();

  SDL_Window *window_;
  SDL_GLContext context_;
  TwBar *tweak_bar_;
  GracefulRainGame game_;
  mojgame::FPSCounter fps_counter_;
};

#endif /* GRACEFULRAIN_GRACEFULRAINAPP_H_ */
