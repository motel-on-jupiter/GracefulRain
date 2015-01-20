/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CORE_SCENE_GRACEFULRAINSCENE_H_
#define CORE_SCENE_GRACEFULRAINSCENE_H_

#include "core/scene/test/TestSceneGraph.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/auxiliary/sdl_aux.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/scene/Scene.h"

class GracefulRainBaseScene :
    public mojgame::BaseScene,
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

#endif /* CORE_SCENE_GRACEFULRAINSCENE_H_ */
