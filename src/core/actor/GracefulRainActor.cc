/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/actor/GracefulRainActor.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

bool GracefulRainActor::Stimulate(mojgame::RippleGLRenderer &renderer) {
  if (appeared()) {
    step_count_ += glm::length(pos() - step_pos_);
    step_pos_ = pos();
    if (step_count_ < step_length_) {
      return true;
    }
    while (step_count_ > step_length_ && step_length_ > 0.0f) {
      step_count_ -= step_length_;
    }
    mojgame::RippleStimulus stimulus;
    glm::vec2 margin = glm::vec2(0.0f, feet_margin_ * 0.5f);
    if (left_landing_) {
      margin.y *= -1.0f;
    }
    margin = glm::rotate(margin, rot());
    left_landing_ = !left_landing_;
    stimulus.pos = pos() + margin;
    stimulus.effect = 1.0f;
    if (!renderer.Receive(stimulus)) {
      mojgame::LOGGER().Error("Failed for renderer to receive stimulus");
      return false;
    }
  }
  return true;
}
