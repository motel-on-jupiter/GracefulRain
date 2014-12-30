/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/production/ProductionScene.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/coroutine_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/scene/Scene.h"

const std::string ProductionScene::kName("Curtain Up");

ProductionScene::ProductionScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), nullptr, tweak_bar),
      ripple_renderer_(),
      rainy_stimulator_(),
      telop_renderer_(),
      renderer_stack_(),
      ccr_param_(nullptr) {
}

bool ProductionScene::OnInitial(const glm::vec2 &window_size) {
  if (!GracefulRainBaseScene::OnInitial(window_size)) {
    return false;
  }
  ripple_renderer_.Initialize(window_size);
  ripple_renderer_.Attach(rainy_stimulator_);
  renderer_stack_.push_back(&ripple_renderer_);
  telop_renderer_.Initialize(window_size);
  return true;
}

void ProductionScene::OnFinal() {
  ccrAbort(ccr_param_);
  renderer_stack_.clear();
  telop_renderer_.Finalize();
  ripple_renderer_.Dettach();
  ripple_renderer_.Finalize();
  return GracefulRainBaseScene::OnFinal();
}

bool ProductionScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);

  ccrAsContParam(ccr_param_);
  ccrBeginContext;
  ccrEndContext(ctx);
  ccrBegin_(ctx);
  while (time() < 2.0f) {
    ccrReturn(true);
  }
  telop_renderer_.Reset("Developed by The Motel on Jupiter", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  while (time() < 5.0f) {
    ccrReturn(true);
  }
  telop_renderer_.Reset("Lost In Rain", glm::vec2(0.5f));
  while (time() < 10.0f) {
    ccrReturn(true);
  }
  renderer_stack_.pop_back();
  while (time() < 2.0f) {
    ccrReturn(true);
  }
  Finish();
  ccrFinish(true);
}

bool ProductionScene::OnRendering(const glm::vec2 &window_size) {
  for (auto it = renderer_stack_.begin(); it != renderer_stack_.end(); ++it) {
    if (!(*it)->Render(window_size)) {
      mojgame::LOGGER().Warn("Failed to render by renderer");
    }
  }
  return true;
}

bool ProductionScene::OnReaction(const SDL_MouseButtonEvent &button,
                                 const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      mojgame::RippleStimulus stimulus;
      stimulus.pos = glm::vec2(static_cast<float>(button.x),
                               static_cast<float>(button.y)) / window_size;
      stimulus.pos.y = 1.0f - stimulus.pos.y;
      stimulus.effect = 1.0f;
      ripple_renderer_.Stimulate(stimulus);
    }
  }
  return true;
}

bool ProductionScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                 const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    mojgame::RippleStimulus stimulus;
    stimulus.pos = glm::vec2(static_cast<float>(motion.x),
                             static_cast<float>(motion.y)) / window_size;
    stimulus.pos.y = 1.0f - stimulus.pos.y;
    stimulus.effect = 1.0f;
    ripple_renderer_.Stimulate(stimulus);
  }
  return true;
}
