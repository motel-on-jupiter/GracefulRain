/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_PHANTOM_H_
#define CORE_ACTOR_PHANTOM_H_

#include "core/actor/GracefulRainActor.h"

class Phantom : public GracefulRainActor {
 public:
  Phantom()
      : GracefulRainActor(kSpeed, 0.0f, 0.0f, kStimulusColor, kStimulusEffect),
        life_(kLifeMax) {
  }
  void ReceiveDamage() {
    if (life_ > 0) {
      life_ -= 1;
      set_stimulus_effect(
          kStimulusEffect
              * (0.5f
                  + 0.5f * static_cast<float>(life_)
                      / static_cast<float>(kLifeMax)));
    }
  }
  bool IsDead() const {
    return life_ == 0;
  }
  void Revive(bool tough, float speed_scale, const glm::vec2 &pos) {
    life_ = tough ? kLifeMaxOnTough : kLifeMax;
    set_stimulus_effect(tough ? kStimulusEffectOnTough : kStimulusEffect);
    set_stimulus_color(tough ? kStimulusColorOnTough : kStimulusColor);
    set_walk_speed(kSpeed * speed_scale);
    set_pos(pos);
  }

 private:
  static const float kSpeed;
  static const glm::vec3 kStimulusColor;
  static const float kStimulusEffect;
  static const int kLifeMax;
  static const float kSpeedOnTough;
  static const glm::vec3 kStimulusColorOnTough;
  static const float kStimulusEffectOnTough;
  static const int kLifeMaxOnTough;

  int life_;
};

#endif /* CORE_ACTOR_PHANTOM_H_ */
