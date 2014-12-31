/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_PRODUCTION_CURTAINUPSCENE_H_
#define CORE_SCENE_PRODUCTION_CURTAINUPSCENE_H_

#include <string>
#include "core/actor/Pablo.h"
#include "core/actor/Rina.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/coroutine_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/catalogue/renderer/TelopRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"

class ProductionScene : public GracefulRainBaseScene {
 public:
  static const std::string kName;

  explicit ProductionScene(TwBar &tweak_bar);
  virtual ~ProductionScene() {
  }

 protected:
  bool OnInitial(const glm::vec2 &window_size);
  void OnFinal();
  bool OnStep(float elapsed_time);
  bool OnRendering(const glm::vec2 &window_size);
  bool OnReaction(const SDL_MouseButtonEvent &button,
                  const glm::vec2 &window_size);
  bool OnReaction(const SDL_MouseMotionEvent &motion,
                  const glm::vec2 &window_size);

 private:
  void Direct();

  mojgame::RippleGLRenderer ripple_renderer_;
  mojgame::RainyRippleStimulator rainy_stimulator_;
  mojgame::GLTelopRenderer telop_renderer_;
  std::vector<mojgame::BaseRenderer *> renderer_stack_;
  void *ccr_param_;
  Rina rina_;
  Pablo pablo_;
};

#endif /* CORE_SCENE_PRODUCTION_CURTAINUPSCENE_H_ */
