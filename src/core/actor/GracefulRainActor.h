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
  GracefulRainActor(float speed, float feet_margin)
      : mojgame::PlanarActor(speed),
        feet_margin_(feet_margin),
        left_landing_(true) {
  }
  void Appear(const glm::vec2 &pos) {
    mojgame::PlanarActor::Appear(pos, 0.0f, glm::vec2());
  }
  void Stimulate(mojgame::RippleGLRenderer &renderer);

 private:
  float feet_margin_;
  bool left_landing_;
};

#endif /* CORE_ACTOR_GRACEFULRAINACTOR_H_ */
