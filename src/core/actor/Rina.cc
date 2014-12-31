/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/actor/Rina.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/entity/PlanarActor.h"
#include "mojgame/includer/glm_include.h"

const float Rina::kSpeed = 0.05f;
const float Rina::kFeetMargin = 0.01f;

void Rina::OnAppearing(const glm::vec2 &pos, float rot, const glm::vec2 &size) {
  UNUSED(pos);
  UNUSED(rot);
  UNUSED(size);

  left_landing_ = true;
}

void Rina::Stimulate(mojgame::RippleGLRenderer &renderer) {
  mojgame::RippleStimulus stimulus;
  glm::vec2 margin = glm::vec2(0.0f, kFeetMargin * 0.5f);
  if (left_landing_) {
    margin.y *= -1.0f;
  }
  margin = glm::rotate(margin, rot());
  left_landing_ = !left_landing_;
  stimulus.pos = pos() + margin;
  stimulus.effect = 1.0f;
  renderer.Receive(stimulus);
}
