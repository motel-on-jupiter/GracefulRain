/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_PABLO_H_
#define CORE_ACTOR_PABLO_H_

#include "core/actor/GracefulRainActor.h"
#include "mojgame/includer/glm_include.h"

class Pablo : public GracefulRainActor {
 public:
  Pablo()
      : GracefulRainActor(kSpeed, kFeetMargin) {
  }

 private:
  static const float kSpeed;
  static const float kFeetMargin;
};

#endif /* CORE_ACTOR_PABLO_H_ */
