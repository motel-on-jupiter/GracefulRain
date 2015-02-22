/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_PHANTOM_H_
#define CORE_ACTOR_PHANTOM_H_

#include "core/actor/GracefulRainActor.h"
#include "mojgame/audio/SePlayer.h"

class Phantom : public GracefulRainActor {
 public:
  Phantom()
      : GracefulRainActor(kSpeed, 0.0f, 0.0f, kStimulusColor, kStimulusEffect),
        voice_pitch_(1.0f),
        life_(kLifeMax) {
    RandomizeVoicePitch();
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
    RandomizeVoicePitch();
    life_ = tough ? kLifeMaxOnTough : kLifeMax;
    set_stimulus_effect(tough ? kStimulusEffectOnTough : kStimulusEffect);
    set_stimulus_color(tough ? kStimulusColorOnTough : kStimulusColor);
    set_walk_speed(kSpeed * speed_scale);
    set_pos(pos);
  }

  float voice_pitch() const {
    return voice_pitch_;
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

  void RandomizeVoicePitch() {
    voice_pitch_ = glm::linearRand(0.9f, 1.2f);
  }

  float voice_pitch_;
  int life_;
};

#endif /* CORE_ACTOR_PHANTOM_H_ */
