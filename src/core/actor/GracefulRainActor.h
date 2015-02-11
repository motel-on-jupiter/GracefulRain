/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_ACTOR_GRACEFULRAINACTOR_H_
#define CORE_ACTOR_GRACEFULRAINACTOR_H_

#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

class GracefulRainActor : public mojgame::PlanarActor {
 public:
  GracefulRainActor(float speed, float step_length, float feet_margin,
                    const glm::vec3 &stimulus_color, float stimulus_effect)
      : mojgame::PlanarActor(speed),
        step_length_(step_length),
        feet_margin_(feet_margin),
        stimulus_color_(stimulus_color),
        stimulus_effect_(stimulus_effect),
        left_landing_(true),
        step_pos_(),
        step_count_(0.0f),
        hop_(false),
        stamp_(false),
        walk_finished_(false) {
  }
  void Appear(const glm::vec2 &pos) {
    mojgame::PlanarActor::Appear(pos, 0.0f, glm::vec2());
  }
  void Hop() {
    hop_ = true;
  }
  void Stamp() {
    stamp_ = true;
  }
  bool Stimulate(mojgame::RippleGLRenderer &renderer);

 protected:
  void OnWalkFinished() {
    walk_finished_ = true;
  }

  void set_stimulus_color(const glm::vec3 &stimulus_color) {
    stimulus_color_ = stimulus_color;
  }
  void set_stimulus_effect(float stimulus_effect) {
    stimulus_effect_ = stimulus_effect;
  }

 private:
  float step_length_;
  float feet_margin_;
  glm::vec3 stimulus_color_;
  float stimulus_effect_;
  bool left_landing_;
  glm::vec2 step_pos_;
  float step_count_;
  bool hop_;
  bool stamp_;
  bool walk_finished_;
};

#endif /* CORE_ACTOR_GRACEFULRAINACTOR_H_ */
