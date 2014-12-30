/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/test/PhantomTestScene.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/gl/gl_planar_rendering.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/scene/Scene.h"

const std::string PhantomTestScene::kName("Phantom Test");

PhantomTestScene::PhantomTestScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), &renderer_, tweak_bar),
      renderer_(),
      phantom_stimulator_() {
}

bool PhantomTestScene::OnInitial(const glm::vec2 &window_size) {
  if (!GracefulRainBaseScene::OnInitial(window_size)) {
    return false;
  }
  phantom_stimulator_.Reset(window_size * 0.5f, 0.0f, 1.0f);
  return true;
}

bool PhantomTestScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);

  return true;
}

bool PhantomTestScene::OnReaction(const SDL_KeyboardEvent &keyboard) {
  if (keyboard.type == SDL_KEYDOWN) {
    if (keyboard.keysym.sym == SDLK_w) {
      renderer_.Dettach();
      renderer_.Attach(phantom_stimulator_);
      phantom_stimulator_.Accel(0.2f);
    }
    if (keyboard.keysym.sym == SDLK_s) {
      renderer_.Dettach();
      renderer_.Attach(phantom_stimulator_);
      phantom_stimulator_.Brake(0.2f);
    }
    if (keyboard.keysym.sym == SDLK_d) {
      phantom_stimulator_.Rotate(-0.2f);
    }
    if (keyboard.keysym.sym == SDLK_a) {
      phantom_stimulator_.Rotate(0.2f);
    }
  }
  return true;
}

bool PhantomTestScene::OnReaction(const SDL_MouseButtonEvent &button,
                                 const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      mojgame::RippleStimulus stimulus;
      stimulus.pos = glm::vec2(static_cast<float>(button.x),
                               static_cast<float>(button.y)) / window_size;
      stimulus.pos.y = 1.0f - stimulus.pos.y;
      stimulus.effect = 1.0f;
      renderer_.Stimulate(stimulus);
    }
  }
  return true;
}

bool PhantomTestScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                 const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    mojgame::RippleStimulus stimulus;
    stimulus.pos = glm::vec2(static_cast<float>(motion.x),
                             static_cast<float>(motion.y)) / window_size;
    stimulus.pos.y = 1.0f - stimulus.pos.y;
    stimulus.effect = 1.0f;
    renderer_.Stimulate(stimulus);
  }
  return true;
}
