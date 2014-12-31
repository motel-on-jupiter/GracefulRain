/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_GRACEFULRAINHUMAN_H_
#define CORE_ACTOR_GRACEFULRAINHUMAN_H_

#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

class GracefulRainHuman : public mojgame::PlanarActor {
 public:
  GracefulRainHuman(float speed, float feet_margin)
      : mojgame::PlanarActor(speed),
        feet_margin_(feet_margin),
        left_landing_(true) {
  }
  void Appear(const glm::vec2 &pos);
  void Stimulate(mojgame::RippleGLRenderer &renderer);

 protected:
  void OnAppearing(const glm::vec2 &pos, float rot, const glm::vec2 &size);

 private:
  float feet_margin_;
  bool left_landing_;
};

#endif /* CORE_ACTOR_GRACEFULRAINHUMAN_H_ */
