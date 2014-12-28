/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_RIPPLESTIMULATOR_PHANTOMRIPPLESTIMULATOR_H_
#define CORE_RIPPLESTIMULATOR_PHANTOMRIPPLESTIMULATOR_H_

#include "mojgame/catalogue/renderer/RippleRenderer.h"

class PhantomRippleStimulator : public mojgame::WalkerRippleStimulator {
 public:
  PhantomRippleStimulator()
      : mojgame::WalkerRippleStimulator() {
  }
  void Reset(const glm::vec2 &pos, float dir, float speed, bool move_forward = true) {
    mojgame::WalkerRippleStimulator::Reset(pos, dir, speed, 0.0f, false, move_forward);
  }
  virtual bool IsDead() const {
    return false;
  }
};

#endif /* CORE_RIPPLESTIMULATOR_PHANTOMRIPPLESTIMULATOR_H_ */
