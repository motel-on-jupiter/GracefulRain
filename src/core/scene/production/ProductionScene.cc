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
const glm::vec2 ProductionScene::kPhantomStartPoss[] = {
  glm::vec2(0.45f, 0.0f),
  glm::vec2(0.55f, 0.0f),
  glm::vec2(0.0f, 0.45f),
  glm::vec2(0.0f, 0.55f),
  glm::vec2(0.45f, 1.0f),
  glm::vec2(0.55f, 1.0f),
  glm::vec2(1.0f, 0.45f),
  glm::vec2(1.0f, 0.55f),
};
const glm::vec2 ProductionScene::kPhantomWalkDests[] = {
  glm::vec2(0.45f, 0.4f),
  glm::vec2(0.55f, 0.4f),
  glm::vec2(0.4f, 0.45f),
  glm::vec2(0.4f, 0.55f),
  glm::vec2(0.45f, 0.6f),
  glm::vec2(0.55f, 0.6f),
  glm::vec2(0.6f, 0.45f),
  glm::vec2(0.6f, 0.55f),
};

ProductionScene::ProductionScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), nullptr, tweak_bar),
      ripple_renderer_(),
      rainy_stimulator_(),
      telop_renderer_(),
      renderer_stack_(),
      ccr_param_(nullptr),
      rina_(),
      pablo_(),
      phantoms_(),
      stimulus_(),
      timer_(0.0f) {
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
  ripple_renderer_.Finalize();
  return GracefulRainBaseScene::OnFinal();
}

void ProductionScene::Direct() {
  int i = 0;
  ccrAsContParam(ccr_param_);
  ccrBeginContext;
  ccrEndContext(ctx);
  ccrBegin_(ctx);
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Developed by The Motel on Jupiter", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  timer_ = 0.0f;
  while (timer_ < 4.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  timer_ = 0.0f;
  while (timer_ < 0.1f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Lost In Rain", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  timer_ = 0.0f;
  while (timer_ < 4.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
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
  rina_.Appear(glm::vec2(0.5f, 1.0f));
  rina_.Walk(glm::vec2(0.5f));
  while (rina_.walking()) {
    ccrReturnV;
  }
  for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    phantoms_[i].Appear(kPhantomStartPoss[i]);
    phantoms_[i].Walk(kPhantomWalkDests[i]);
  }
  timer_ = 0.0f;
  while (timer_ < 60.0f) {
    for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
      if (glm::length2(phantoms_[i].pos() - stimulus_.pos) < 0.1f * 0.1f) {
        phantoms_[i].set_pos(kPhantomStartPoss[i]);
      }
    }
    for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
      if (!phantoms_[i].walking()) {
        telop_renderer_.Reset("Game Over", glm::vec2(0.5f));
        renderer_stack_.push_back(&telop_renderer_);
        while (true) {
          ccrReturnV;
        }
      }
    }
    ccrReturnV;
  }
  for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    phantoms_[i].Disappear();
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  pablo_.Appear(glm::vec2(0.55f, 1.0f));
  pablo_.Walk(glm::vec2(0.55f, 0.5f));
  while (pablo_.walking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 0.0f));
  pablo_.Walk(glm::vec2(0.55f, 0.0f));
  while (rina_.walking() || pablo_.walking()) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Fin", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  while (true) {
    ccrReturnV;
  }
  ccrFinishV;
}

bool ProductionScene::OnStep(float elapsed_time) {
  timer_ += elapsed_time;
  if (!finished()) {
    Direct();
  }
  if (!rina_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Rina");
    return false;
  }
  if (rina_.walking()) {
    if (!rina_.Stimulate(ripple_renderer_)) {
      mojgame::LOGGER().Error("Failed for Rina to stimulate");
      return false;
    }
  }
  if (!pablo_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Pablo");
    return false;
  }
  if (pablo_.walking()) {
    if (!pablo_.Stimulate(ripple_renderer_)) {
      mojgame::LOGGER().Error("Failed for Pablo to stimulate");
      return false;
    }
  }
  for (int i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    if (!phantoms_[i].Step(elapsed_time)) {
      mojgame::LOGGER().Error("Failed to step Phantom (idx: %d)", i);
      return false;
    }
    if (phantoms_[i].walking()) {
      if (!phantoms_[i].Stimulate(ripple_renderer_)) {
        mojgame::LOGGER().Error("Failed for Phantom to stimulate (idx: %d)", i);
        return false;
      }
    }
  }
  if (stimulus_.effect > 0.0f) {
    ripple_renderer_.Receive(stimulus_);
    stimulus_.effect = 0.0f;
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
      glm::vec2 pos = glm::vec2(static_cast<float>(button.x),
                               static_cast<float>(button.y)) / window_size;
      pos.y = 1.0f - pos.y;
      stimulus_.pos = pos + glm::diskRand(0.1f);
      stimulus_.effect = 1.0f;
    }
  }
  return true;
}

bool ProductionScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                 const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    glm::vec2 pos = glm::vec2(static_cast<float>(motion.x),
                             static_cast<float>(motion.y)) / window_size;
    pos.y = 1.0f - pos.y;
    stimulus_.pos = pos + glm::diskRand(0.1f);
    stimulus_.effect = 1.0f;
  }
  return true;
}
