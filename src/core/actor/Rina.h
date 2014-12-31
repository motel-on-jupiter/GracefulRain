/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_RINA_H_
#define CORE_ACTOR_RINA_H_

#include "core/actor/GracefulRainActor.h"

class Rina : public GracefulRainActor {
 public:
  Rina()
      : GracefulRainActor(kSpeed, kFeetMargin) {
  }

 private:
  static const float kSpeed;
  static const float kFeetMargin;
};

#endif /* CORE_ACTOR_RINA_H_ */
