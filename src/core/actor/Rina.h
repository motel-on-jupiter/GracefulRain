/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_RINA_H_
#define CORE_ACTOR_RINA_H_

#include "mojgame/entity/PlanarEntity.h"

class Rina : public mojgame::PlanarEntity {
 public:
  Rina() : mojgame::WalkerRippleStimulator() {
  }
  void Reset(const glm::vec2 &pos, float dir, float speed, bool move_forward = true) {
    mojgame::WalkerRippleStimulator::Reset(pos, dir, speed, 0.0f, false, move_forward);
  }
  virtual bool IsDead() const {
    return false;
  }
};

#endif /* CORE_ACTOR_RINA_H_ */
