/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_RINA_H_
#define CORE_ACTOR_RINA_H_

#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

class Rina : public mojgame::PlanarActor {
 public:
  Rina()
      : mojgame::PlanarActor(kSpeed),
        left_landing_(true) {
  }
  void Stimulate(mojgame::RippleGLRenderer &renderer);

 protected:
  void OnAppearing(const glm::vec2 &pos, float rot, const glm::vec2 &size);

 private:
  static const float kSpeed;
  static const float kFeetMargin;

  bool left_landing_;
};

#endif /* CORE_ACTOR_RINA_H_ */
