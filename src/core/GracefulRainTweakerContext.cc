/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/GracefulRainTweakerContext.h"

GracefulRainTweakerContext::GracefulRainTweakerContext()
    : system_actual_fps(0),
      system_time_speed(1.0f),
      ripples_rgb_filter(0.1f) {
}

GracefulRainTweakerContext::~GracefulRainTweakerContext() {
}

GracefulRainTweakerContext tweaker_ctx;
