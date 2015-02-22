/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_GRACEFULRAINTWEAKERCONTEXT_H_
#define CORE_GRACEFULRAINTWEAKERCONTEXT_H_

#include "mojgame/includer/glm_include.h"

struct GracefulRainTweakerContext {
  int system_actual_fps;
  float system_time_speed;
  glm::vec3 ripples_rgb_filter;

  GracefulRainTweakerContext();
  ~GracefulRainTweakerContext();
};

extern GracefulRainTweakerContext tweaker_ctx;

#endif /* CORE_GRACEFULRAINTWEAKERCONTEXT_H_ */
