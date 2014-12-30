/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_TEST_PHANTOMTESTSCENE_H_
#define CORE_SCENE_TEST_PHANTOMTESTSCENE_H_

#include <string>
#include "core/scene/GracefulRainScene.h"
#include "core/ripple_stimulator/PhantomRippleStimulator.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"

class PhantomTestScene : public GracefulRainBaseScene {
 public:
  static const std::string kName;

  explicit PhantomTestScene(TwBar &tweak_bar);
  virtual ~PhantomTestScene() {
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
  mojgame::RippleGLRenderer renderer_;
  PhantomRippleStimulator phantom_stimulator_;
};

#endif /* CORE_SCENE_TEST_PHANTOMTESTSCENE_H_ */
