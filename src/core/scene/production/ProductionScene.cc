/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/production/ProductionScene.h"
#include <string>
#include <vector>
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/coroutine_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/scene/Scene.h"

const std::string ProductionScene::kName("Production");

ProductionScene::ProductionScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), nullptr, tweak_bar),
      ripple_renderer_(),
      rainy_stimulator_(),
      telop_renderer_(),
      renderer_stack_(),
      ccr_param_(nullptr),
      rina_(),
      pablo_(),
      phantoms_() {
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

void ProductionScene::Direct() {
  int i = 0;
  ccrAsContParam(ccr_param_);
  ccrBeginContext;
  ccrEndContext(ctx);
  ccrBegin_(ctx);
  while (time() < 2.0f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Developed by The Motel on Jupiter", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  while (time() < 7.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  while (time() < 1.0f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Lost In Rain", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  while (time() < 12.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  while (time() < 2.0f) {
    ccrReturnV;
  }
  rina_.Appear(glm::vec2(1.0f, 0.0f));
  rina_.Walk(glm::vec2(0.0f, 1.0f));
  while (rina_.walking()) {
    ccrReturnV;
  }
  pablo_.Appear(glm::vec2(1.0f, 0.0f));
  pablo_.Walk(glm::vec2(0.0f, 1.0f));
  while (pablo_.walking()) {
    ccrReturnV;
  }
  rina_.Appear(glm::vec2(1.0f, 0.0f));
  rina_.Walk(glm::vec2(0.5f));
  while (rina_.walking()) {
    ccrReturnV;
  }
  phantoms_[0].Appear(glm::vec2(0.45f, 0.0f));
  phantoms_[0].Walk(glm::vec2(0.45f, 0.4f));
  phantoms_[1].Appear(glm::vec2(0.55f, 0.0f));
  phantoms_[1].Walk(glm::vec2(0.55f, 0.4f));
  phantoms_[2].Appear(glm::vec2(0.0f, 0.45f));
  phantoms_[2].Walk(glm::vec2(0.4f, 0.45f));
  phantoms_[3].Appear(glm::vec2(0.0f, 0.55f));
  phantoms_[3].Walk(glm::vec2(0.4f, 0.55f));
  phantoms_[4].Appear(glm::vec2(0.45f, 1.0f));
  phantoms_[4].Walk(glm::vec2(0.45f, 0.6f));
  phantoms_[5].Appear(glm::vec2(0.55f, 1.0f));
  phantoms_[5].Walk(glm::vec2(0.55f, 0.6f));
  phantoms_[6].Appear(glm::vec2(1.0f, 0.45f));
  phantoms_[6].Walk(glm::vec2(0.6f, 0.45f));
  phantoms_[7].Appear(glm::vec2(1.0f, 0.55f));
  phantoms_[7].Walk(glm::vec2(0.6f, 0.55f));
  for (i=0; i<ARRAYSIZE(phantoms_); ++i) {
    while (phantoms_[i].walking()) {
      ccrReturnV;
    }
  }
  telop_renderer_.Reset("Fin", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  while (true) {
    ccrReturnV;
  }
  ccrFinishV;
}

bool ProductionScene::OnStep(float elapsed_time) {
  if (!finished()) {
    Direct();
  }
  if (!rina_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Rina");
    return false;
  }
  if (rina_.walking()) {
    rina_.Stimulate(ripple_renderer_);
  }
  if (!pablo_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Pablo");
    return false;
  }
  if (pablo_.walking()) {
    pablo_.Stimulate(ripple_renderer_);
  }
  for (int i=0; i<ARRAYSIZE(phantoms_); ++i) {
    if (!phantoms_[i].Step(elapsed_time)) {
      mojgame::LOGGER().Error("Failed to step Phantom (idx: %d)", i);
      return false;
    }
    if (phantoms_[i].walking()) {
      phantoms_[i].Stimulate(ripple_renderer_);
    }
  }
  return true;
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
      ripple_renderer_.Receive(stimulus);
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
    ripple_renderer_.Receive(stimulus);
  }
  return true;
}
