/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_TEST_RIPPLETESTSCENE_H_
#define CORE_SCENE_TEST_RIPPLETESTSCENE_H_

#include <string>
#include "core/GracefulRainGame.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"

class RippleTestSceneRenderer : public mojgame::RippleGLRenderer {
 public:
  RippleTestSceneRenderer();
  virtual ~RippleTestSceneRenderer() {
  }
};

class RippleTestScene : public GracefulRainBaseScene {
 public:
  static const std::string kName;

  explicit RippleTestScene(TwBar &tweak_bar);
  virtual ~RippleTestScene() {
  }

 protected:
  virtual bool OnInitial(const glm::vec2 &window_size);
  virtual bool OnStep(float elapsed_time);
  virtual bool OnReaction(const SDL_KeyboardEvent &keyboard);
  virtual bool OnReaction(const SDL_MouseButtonEvent &button,
                          const glm::vec2 &window_size);
  virtual bool OnReaction(const SDL_MouseMotionEvent &motion,
                          const glm::vec2 &window_size);

 private:
  RippleTestSceneRenderer renderer_;
  mojgame::RainyRippleStimulator rainy_stimulator_;
  mojgame::WalkerRippleStimulator walker_stimulator_;
};

#endif /* CORE_SCENE_TEST_RIPPLETESTSCENE_H_ */
