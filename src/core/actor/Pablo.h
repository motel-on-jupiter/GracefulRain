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

#endif /* CORE_ACTOR_PABLO_H_ */
