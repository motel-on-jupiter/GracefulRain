/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_RINA_H_
#define CORE_ACTOR_RINA_H_

#include "core/actor/GracefulRainHuman.h"

class Rina : public GracefulRainHuman {
 public:
  Rina()
      : GracefulRainHuman(kSpeed, kFeetMargin) {
  }

 private:
  static const float kSpeed;
  static const float kFeetMargin;
};

#endif /* CORE_ACTOR_RINA_H_ */
