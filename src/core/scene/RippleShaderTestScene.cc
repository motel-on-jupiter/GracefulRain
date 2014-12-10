/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/RippleShaderTestScene.h"
#include "mojgame/auxiliary/gl/gl_planar_rendering.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/gl/GradationalGLRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/scene/Scene.h"

RippleShaderTestScene::RippleShaderTestScene(TwBar &tweak_bar)
    : GracefulRainBaseScene("Ripple Shader Test", tweak_bar),
      dragging_pos_() {
}

bool RippleShaderTestScene::OnInitial() {
  return true;
}

void RippleShaderTestScene::OnFinal() {
}

bool RippleShaderTestScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);

  return true;
}

bool RippleShaderTestScene::OnReaction(const SDL_MouseButtonEvent &button,
                                       const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      dragging_pos_ = glm::vec2(static_cast<float>(button.x),
                                window_size.y - static_cast<float>(button.y));
    } else {
      dragging_pos_ = glm::vec2(0.0f);
    }
  }
  return true;
}

bool RippleShaderTestScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                       const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    dragging_pos_ = glm::vec2(static_cast<float>(motion.x),
                              window_size.y - static_cast<float>(motion.y));
  }
  return true;
}

#define SHADER_SOURCE(version, ...) "#version " #version "\n" #__VA_ARGS__

static const char kRainVShaderSource[] = SHADER_SOURCE(330 core,
layout(location = 0) in vec2 coord;
layout(location = 1) in vec2 uv;

out vec2 vertexUv;

void main(void) {
  gl_Position.xy = coord;
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
  vertexUv = uv;
}
);

static const char kRainFShaderSource[] = SHADER_SOURCE(330 core,
in vec2 vertexUv;

layout (location = 0) out vec4 fragmentColor;

uniform sampler2D prev0_tex;
uniform sampler2D prev1_tex;
uniform vec2 windowSize;
uniform vec2 dropPos;

const float kEpsilon = 0.0000001;

void main(void) {
  vec2 texelSize = vec2(1.0) / windowSize;
  if (length(dropPos) > kEpsilon) {
    if ((abs(dropPos.x - vertexUv.x) < texelSize.x * 0.6) &&
        (abs(dropPos.y - vertexUv.y) < texelSize.y * 0.6)) {
      fragmentColor = vec4(1.0);
      return;
    }
  }

  fragmentColor = texture2D(prev0_tex, vertexUv);
  if (vertexUv.x - texelSize.x > 0.0 && vertexUv.y - texelSize.y > 0.0) {
    fragmentColor += texture2D(prev0_tex, vertexUv + texelSize * vec2(-1.0, -1.0));
  } else {
    fragmentColor += texture2D(prev0_tex, vertexUv);
  }
  if (vertexUv.x - texelSize.x > 0.0 && vertexUv.y + texelSize.y < 1.0) {
    fragmentColor += texture2D(prev0_tex, vertexUv + texelSize * vec2(-1.0, 1.0));
  } else {
    fragmentColor += texture2D(prev0_tex, vertexUv);
  }
  if (vertexUv.x + texelSize.x < 1.0 && vertexUv.y - texelSize.y > 0.0) {
    fragmentColor += texture2D(prev0_tex, vertexUv + texelSize * vec2(1.0, -1.0));
  } else {
    fragmentColor += texture2D(prev0_tex, vertexUv);
  }
  if (vertexUv.x + texelSize.x < 1.0 && vertexUv.y + texelSize.y < 1.0) {
    fragmentColor += texture2D(prev0_tex, vertexUv + texelSize * vec2(1.0, 1.0));
  } else {
    fragmentColor += texture2D(prev0_tex, vertexUv);
  }
  fragmentColor *= 0.25;
  fragmentColor *= 0.79;
  fragmentColor += texture2D(prev0_tex, vertexUv) - texture2D(prev1_tex, vertexUv);
  fragmentColor.a = 1.0;
}
);

RippleShaderTestSceneGLRenderer::RippleShaderTestSceneGLRenderer(
    RippleShaderTestScene &scene)
    : mojgame::BaseSceneRenderer(scene),
      gradational_renderer_(kRainVShaderSource, kRainFShaderSource, 3),
      drop_pos_() {
}

bool RippleShaderTestSceneGLRenderer::OnInitial(const glm::vec2 &window_size) {
  return gradational_renderer_.Initialize(window_size);
}

void RippleShaderTestSceneGLRenderer::OnFinal() {
  gradational_renderer_.Finalize();
}

bool RippleShaderTestSceneGLRenderer::OnRendering(
    const glm::vec2 &window_size) {
  mojgame::gl_shader::set_uniform_v2(gradational_renderer_.gradation_program(),
                                     "windowSize", window_size);
  drop_pos_ = GetScene().dragging_pos();
  mojgame::gl_shader::set_uniform_v2(gradational_renderer_.gradation_program(),
                                     "dropPos", drop_pos_ / window_size);
  return gradational_renderer_.Render(window_size);
}
