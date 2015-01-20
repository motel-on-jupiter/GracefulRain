/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_RINA_H_
#define CORE_ACTOR_RINA_H_

#include "core/actor/GracefulRainActor.h"

class Rina : public GracefulRainActor {
 public:
  Rina()
      : GracefulRainActor(kSpeed, kStepLength, kFeetMargin, kStimulusColor,
                          kStimulusEffect) {
  }

 private:
  static const float kSpeed;
  static const float kStepLength;
  static const float kFeetMargin;
  static const glm::vec3 kStimulusColor;
  static const float kStimulusEffect;
};

#endif /* CORE_ACTOR_RINA_H_ */
