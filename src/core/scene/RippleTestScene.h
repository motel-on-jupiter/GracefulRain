/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_RIPPLETESTSCENE_H_
#define CORE_SCENE_RIPPLETESTSCENE_H_

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
  explicit RippleTestScene(TwBar &tweak_bar);
  virtual ~RippleTestScene() {
  }

 protected:
  virtual bool OnStep(float elapsed_time);
  virtual bool OnReaction(const SDL_MouseButtonEvent &button,
                          const glm::vec2 &window_size);
  virtual bool OnReaction(const SDL_MouseMotionEvent &motion,
                          const glm::vec2 &window_size);

 private:
  RippleTestSceneRenderer renderer_;
};

#endif /* CORE_SCENE_RIPPLETESTSCENE_H_ */
