/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/actor/GracefulRainHuman.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

void GracefulRainHuman::OnAppearing(const glm::vec2 &pos, float rot, const glm::vec2 &size) {
  UNUSED(pos);
  UNUSED(rot);
  UNUSED(size);

  left_landing_ = true;
}

void GracefulRainHuman::Appear(const glm::vec2 &pos) {
  mojgame::PlanarActor::Appear(pos, 0.0f, glm::vec2());
}

void GracefulRainHuman::Stimulate(mojgame::RippleGLRenderer &renderer) {
  mojgame::RippleStimulus stimulus;
  glm::vec2 margin = glm::vec2(0.0f, feet_margin_ * 0.5f);
  if (left_landing_) {
    margin.y *= -1.0f;
  }
  margin = glm::rotate(margin, rot());
  left_landing_ = !left_landing_;
  stimulus.pos = pos() + margin;
  stimulus.effect = 1.0f;
  renderer.Receive(stimulus);
}
