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
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneRenderer.h"
#include "mojgame/scene/SceneSuite.h"

class GracefulRainBaseScene : public mojgame::BaseScene,
    public mojgame::sdl_aux::KeyboardEventListenerInterface,
    public mojgame::sdl_aux::MouseEventListenerInterface,
    public mojgame::NonCopyable<GracefulRainBaseScene> {
 public:
  GracefulRainBaseScene(const char *name, TwBar &tweak_bar)
      : mojgame::BaseScene(name),
        tweak_bar_(tweak_bar) {
  }
  virtual ~GracefulRainBaseScene() {
  }

  bool React(const SDL_KeyboardEvent &keyboard) {
    return OnReaction(keyboard);
  }
  bool React(const SDL_MouseMotionEvent &motion, const glm::vec2 &window_size) {
    return OnReaction(motion, window_size);
  }
  bool React(const SDL_MouseButtonEvent &button, const glm::vec2 &window_size) {
    return OnReaction(button, window_size);
  }
  bool React(const SDL_MouseWheelEvent &wheel, const glm::vec2 &window_size) {
    return OnReaction(wheel, window_size);
  }

 protected:
  virtual bool OnReaction(const SDL_KeyboardEvent &keyboard) {
    UNUSED(keyboard);

    return true;
  }
  virtual bool OnReaction(const SDL_MouseMotionEvent &motion,
                          const glm::vec2 &window_size) {
    UNUSED(motion);
    UNUSED(window_size);

    return true;
  }
  virtual bool OnReaction(const SDL_MouseButtonEvent &button,
                          const glm::vec2 &window_size) {
    UNUSED(button);
    UNUSED(window_size);

    return true;
  }
  virtual bool OnReaction(const SDL_MouseWheelEvent &wheel,
                          const glm::vec2 &window_size) {
    UNUSED(wheel);
    UNUSED(window_size);

    return true;
  }

  TwBar &tweak_bar() {
    return tweak_bar_;
  }

 private:
  TwBar &tweak_bar_;
};

class GracefulRainSceneSuite : public mojgame::BaseSceneSuite {
 public:
  GracefulRainSceneSuite(GracefulRainBaseScene &scene,
                         mojgame::BaseSceneRenderer &renderer)
      : mojgame::BaseSceneSuite(*static_cast<mojgame::BaseScene *>(&scene),
                                renderer) {
  }
  ~GracefulRainSceneSuite() {
  }

  GracefulRainBaseScene &GetScene() {
    return reinterpret_cast<GracefulRainBaseScene &>(scene());
  }
};

class GracefulRainGame : public mojgame::NonCopyable<GracefulRainGame> {
 public:
  GracefulRainGame(TwBar &tweak_bar);
  ~GracefulRainGame();

  void Initialize();
  void Finalize();
  bool Step(float elapsed_time);
  bool Render(const glm::vec2 &window_size);
  bool React(const SDL_KeyboardEvent &keyboard, const glm::vec2 &window_size);
  bool React(const SDL_MouseMotionEvent &motion, const glm::vec2 &window_size);
  bool React(const SDL_MouseButtonEvent &button, const glm::vec2 &window_size);
  bool React(const SDL_MouseWheelEvent &wheel, const glm::vec2 &window_size);

  bool ongoing() const {
    return ongoing_;
  }

 private:
  std::vector<GracefulRainSceneSuite *> scene_suites_;
  TwBar &tweak_bar_;
  bool ongoing_;
  int current_;
  unsigned int cursor_;
};

#endif /* CORE_GRACEFULRAINGAME_H_ */
