/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CORE_GRACEFULRAINGAME_H_
#define CORE_GRACEFULRAINGAME_H_

#include <string>
#include <vector>
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/auxiliary/sdl_aux.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/sdl_include.h"

class GracefulRainBaseGameScene :
    public mojgame::sdl_aux::KeyboardEventListenerInterface,
    public mojgame::sdl_aux::MouseEventListenerInterface {
 public:
  GracefulRainBaseGameScene(const char *name)
      : name_(name) {
  }
  virtual ~GracefulRainBaseGameScene() {
  }

  virtual int Initialize(const glm::vec2 &window_size, TwBar &tweak_bar) = 0;
  virtual void Finalize(TwBar &tweak_bar) = 0;
  virtual void Update(float elapsed_time, const glm::vec2 &window_size) = 0;
  virtual void Draw(const glm::vec2 &window_size) = 0;
  virtual void React(const SDL_KeyboardEvent &keyboard) {
    UNUSED(keyboard);
  }
  virtual void React(const SDL_MouseMotionEvent &motion) {
    UNUSED(motion);
  }
  virtual void React(const SDL_MouseButtonEvent &button) {
    UNUSED(button);
  }
  virtual void React(const SDL_MouseWheelEvent &wheel) {
    UNUSED(wheel);
  }

  const std::string &name() const {
    return name_;
  }

 private:
  std::string name_;
};

class GracefulRainGame {
 public:
  GracefulRainGame();
  ~GracefulRainGame();

  int Initialize();
  void Finalize(TwBar &tweak_bar);
  void Update(float elapsed_time, const glm::vec2 &window_size);
  void Draw(const glm::vec2 &window_size);
  bool React(const SDL_KeyboardEvent &keyboard, const glm::vec2 &window_size, TwBar &tweak_bar);
  bool React(const SDL_MouseMotionEvent &motion, const glm::vec2 &window_size);
  bool React(const SDL_MouseButtonEvent &button, const glm::vec2 &window_size);
  bool React(const SDL_MouseWheelEvent &wheel, const glm::vec2 &window_size);

  bool ongoing() const {
    return ongoing_;
  }

 private:
  std::vector<GracefulRainBaseGameScene *> scenes_;
  GracefulRainBaseGameScene *current_scene_;
  unsigned int cursor_;
  bool ongoing_;
};

#endif /* CORE_GRACEFULRAINGAME_H_ */
