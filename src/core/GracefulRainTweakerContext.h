/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_GRACEFULRAINTWEAKERCONTEXT_H_
#define CORE_GRACEFULRAINTWEAKERCONTEXT_H_

struct GracefulRainTweakerContext {
  int system_actual_fps;
  float system_time_speed;

  GracefulRainTweakerContext();
  ~GracefulRainTweakerContext();
};

extern GracefulRainTweakerContext tweaker_ctx;

#endif /* CORE_GRACEFULRAINTWEAKERCONTEXT_H_ */
