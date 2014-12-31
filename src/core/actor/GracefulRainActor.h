/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_GRACEFULRAINACTOR_H_
#define CORE_ACTOR_GRACEFULRAINACTOR_H_

#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

class GracefulRainActor : public mojgame::PlanarActor {
 public:
  GracefulRainActor(float speed, float step_length, float feet_margin)
      : mojgame::PlanarActor(speed),
        step_length_(step_length),
        feet_margin_(feet_margin),
        left_landing_(true),
        step_pos_(),
        step_count_(0.0f) {
  }
  void Appear(const glm::vec2 &pos) {
    mojgame::PlanarActor::Appear(pos, 0.0f, glm::vec2());
  }
  bool Stimulate(mojgame::RippleGLRenderer &renderer);

 private:
  float step_length_;
  float feet_margin_;
  bool left_landing_;
  glm::vec2 step_pos_;
  float step_count_;
};

#endif /* CORE_ACTOR_GRACEFULRAINACTOR_H_ */
