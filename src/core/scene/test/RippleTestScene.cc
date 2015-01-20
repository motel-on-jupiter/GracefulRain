/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/test/RippleTestScene.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/gl/gl_planar_rendering.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/scene/Scene.h"

RippleTestSceneRenderer::RippleTestSceneRenderer()
    : mojgame::RippleGLRenderer() {
}

const std::string RippleTestScene::kName("Ripple Test");

RippleTestScene::RippleTestScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), tweak_bar),
      renderer_(),
      rainy_stimulator_(glm::vec3(1.0f), glm::vec2(0.0f, 1.0f)) {
}

bool RippleTestScene::OnInitial(const glm::vec2 &window_size) {
  if (!renderer_.Initialize(window_size)) {
    return false;
  }
  renderer_.Attach(rainy_stimulator_);
  return true;
}

void RippleTestScene::OnFinal() {
  renderer_.DettachAll();
  renderer_.Finalize();
}

bool RippleTestScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);
  return true;
}

bool RippleTestScene::OnRendering(const glm::vec2 &window_size) {
  return renderer_.Render(window_size);
}

bool RippleTestScene::OnReaction(const SDL_MouseButtonEvent &button,
                                 const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      mojgame::RippleStimulus stimulus;
      stimulus.pos = glm::vec2(static_cast<float>(button.x),
                               static_cast<float>(button.y)) / window_size;
      stimulus.pos.y = 1.0f - stimulus.pos.y;
      stimulus.color = glm::vec3(1.0f, 0.0f, 0.0f);
      stimulus.effect = 1.0f;
      renderer_.Receive(stimulus);
    }
  }
  return true;
}

bool RippleTestScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                 const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    mojgame::RippleStimulus stimulus;
    stimulus.pos = glm::vec2(static_cast<float>(motion.x),
                             static_cast<float>(motion.y)) / window_size;
    stimulus.pos.y = 1.0f - stimulus.pos.y;
    stimulus.color = glm::vec3(1.0f, 0.0f, 0.0f);
    stimulus.effect = 1.0f;
    renderer_.Receive(stimulus);
  }
  return true;
}
