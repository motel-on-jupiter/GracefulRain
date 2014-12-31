/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/actor/GracefulRainActor.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

void GracefulRainActor::Stimulate(mojgame::RippleGLRenderer &renderer) {
  mojgame::RippleStimulus stimulus;
  glm::vec2 margin = glm::vec2(0.0f, feet_margin_ * 0.5f);
  if (left_landing_) {
    margin.y *= -1.0f;
  }
  margin = glm::rotate(margin, rot());
  left_landing_ = !left_landing_;
  stimulus.pos = pos() + margin;
  stimulus.effect = 1.0f;
  renderer.Receive(stimulus);
}
