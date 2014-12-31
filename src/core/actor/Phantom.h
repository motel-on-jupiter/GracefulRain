/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_PHANTOM_H_
#define CORE_ACTOR_PHANTOM_H_

#include "core/actor/GracefulRainActor.h"

class Phantom : public GracefulRainActor {
 public:
  Phantom()
      : GracefulRainActor(kSpeed, 0.0f, 0.0f) {
  }

 private:
  static const float kSpeed;
};

#endif /* CORE_ACTOR_PHANTOM_H_ */
