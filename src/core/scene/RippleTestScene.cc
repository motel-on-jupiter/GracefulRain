/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/RippleTestScene.h"
#include "core/GracefulRainGame.h"
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

RippleTestScene::RippleTestScene(TwBar &tweak_bar)
    : GracefulRainBaseScene("Ripple Test", &renderer_, tweak_bar) {
}

bool RippleTestScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);

  return true;
}

bool RippleTestScene::OnReaction(const SDL_MouseButtonEvent &button,
                                 const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      mojgame::RippleStimulus stimulus;
      stimulus.pos = glm::vec2(static_cast<float>(button.x),
                               static_cast<float>(button.y)) / window_size;
      stimulus.pos.y = 1.0f - stimulus.pos.y;
      stimulus.force = 1.0f;
      renderer_.Stimulate(stimulus);
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
    stimulus.force = 1.0f;
    renderer_.Stimulate(stimulus);
  }
  return true;
}
