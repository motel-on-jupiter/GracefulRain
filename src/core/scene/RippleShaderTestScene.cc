/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/RippleShaderTestScene.h"
#include "mojgame/auxiliary/gl/gl_planar_rendering.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/csyntax_aux.h"
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

uniform sampler2D texture0;
uniform sampler2D texture1;
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

  fragmentColor = texture2D(texture1, vertexUv);
  if (vertexUv.x - texelSize.x > 0.0 && vertexUv.y - texelSize.y > 0.0) {
    fragmentColor += texture2D(texture1, vertexUv + texelSize * vec2(-1.0, -1.0));
  } else {
    fragmentColor += texture2D(texture1, vertexUv);
  }
  if (vertexUv.x - texelSize.x > 0.0 && vertexUv.y + texelSize.y < 1.0) {
    fragmentColor += texture2D(texture1, vertexUv + texelSize * vec2(-1.0, 1.0));
  } else {
    fragmentColor += texture2D(texture1, vertexUv);
  }
  if (vertexUv.x + texelSize.x < 1.0 && vertexUv.y - texelSize.y > 0.0) {
    fragmentColor += texture2D(texture1, vertexUv + texelSize * vec2(1.0, -1.0));
  } else {
    fragmentColor += texture2D(texture1, vertexUv);
  }
  if (vertexUv.x + texelSize.x < 1.0 && vertexUv.y + texelSize.y < 1.0) {
    fragmentColor += texture2D(texture1, vertexUv + texelSize * vec2(1.0, 1.0));
  } else {
    fragmentColor += texture2D(texture1, vertexUv);
  }
  fragmentColor *= 0.25;
  fragmentColor *= 0.79;
  fragmentColor += texture2D(texture1, vertexUv) - texture2D(texture0, vertexUv);
  fragmentColor.a = 1.0;
}
);

static const char kBlitVShaderSource[] = SHADER_SOURCE(330 core,
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

static const char kBlitFShaderSource[] = SHADER_SOURCE(330 core,
in vec2 vertexUv;

layout (location = 0) out vec4 fragmentColor;

uniform sampler2D texture;

void main(void) {
  fragmentColor = texture2D(texture, vertexUv);
  fragmentColor.rgb *= 5.0;
}
);

RippleShaderTestSceneGLRenderer::RippleShaderTestSceneGLRenderer(
    RippleShaderTestScene &scene)
    : mojgame::BaseSceneRenderer(scene),
      rain_shader_program_(0),
      blit_shader_program_(0),
      framebuf_(),
      vertex_array_(0),
      vertex_buffer_(0),
      uv_buffer_(0),
      current_texture_(0),
      drop_pos_() {
}

bool RippleShaderTestSceneGLRenderer::OnInitial(const glm::vec2 &window_size) {
  mojgame::gl_shader::program_t program;
  if (!mojgame::gl_shader::build_program(kRainVShaderSource, kRainFShaderSource,
                                         program)) {
    mojgame::LOGGER().Error("Failed to link shader program to draw rain");
    OnFinal();
    return false;
  }
  rain_shader_program_ = program;
  if (!mojgame::gl_shader::build_program(kBlitVShaderSource, kBlitFShaderSource,
                                         program)) {
    mojgame::LOGGER().Error("Failed to link shader program to blit texture");
    OnFinal();
    return false;
  }
  blit_shader_program_ = program;

  // Set up draw settings
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Set up frame buffer
  if (framebuf_.SetUp(window_size, 3) < 0) {
    mojgame::LOGGER().Error("Failed to set up the frame buffer");
    return false;
  }
  for (auto it = framebuf_.colortexs().begin();
      it != framebuf_.colortexs().end(); ++it) {
    glBindTexture(GL_TEXTURE_2D, *it);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  // Set up vertex objects for blooming
  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);
  glGenBuffers(1, &vertex_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  static const GLfloat kEffectCoords[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, };
  glBufferData(GL_ARRAY_BUFFER, sizeof(kEffectCoords), kEffectCoords,
  GL_STATIC_DRAW);
  glGenBuffers(1, &uv_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
  static const GLfloat kEffectUV[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      1.0f };
  glBufferData(GL_ARRAY_BUFFER, sizeof(kEffectUV), kEffectUV, GL_STATIC_DRAW);
  return true;
}

void RippleShaderTestSceneGLRenderer::OnFinal() {
  // Delete drawing objects
  glDeleteBuffers(1, &uv_buffer_);
  glDeleteBuffers(1, &vertex_buffer_);
  glDeleteVertexArrays(1, &vertex_array_);

  // Clean up frame buffer
  framebuf_.CleanUp();

  // Reset draw settings
  glDisable(GL_CULL_FACE);
  glUseProgram(0);
  mojgame::gl_shader::delete_program(rain_shader_program_);
  rain_shader_program_ = 0;
}

void RippleShaderTestSceneGLRenderer::RenderOnStep1(
    const glm::vec2 &window_size) {
  GLuint prev = (current_texture_ == 0) ? 2 : (current_texture_ - 1);
  GLuint current = current_texture_;
  GLuint next = (current_texture_ == 2) ? 0 : (current_texture_ + 1);

  glUseProgram(rain_shader_program_);
  mojgame::gl_rendering::bind_2d_texture(GL_TEXTURE0,
                                         framebuf_.colortexs()[prev]);
  mojgame::gl_rendering::bind_2d_texture(GL_TEXTURE1,
                                         framebuf_.colortexs()[current]);
  mojgame::gl_shader::set_uniform_i(rain_shader_program_, "texture0", 0);
  mojgame::gl_shader::set_uniform_i(rain_shader_program_, "texture1", 1);
  mojgame::gl_shader::set_uniform_v2(rain_shader_program_, "windowSize",
                                     window_size);
  drop_pos_ = GetScene().dragging_pos();
  mojgame::gl_shader::set_uniform_v2(rain_shader_program_, "dropPos",
                                     drop_pos_ / window_size);

  mojgame::glBindDrawFramebuffer(framebuf_.name());
  mojgame::glFramebufferDrawColorAttachment(next);

  glDrawArrays(GL_QUADS, 0, 4);
  current_texture_ = next;
}

void RippleShaderTestSceneGLRenderer::RenderOnStep2() {
  glUseProgram(blit_shader_program_);
  mojgame::gl_rendering::bind_2d_texture(
      GL_TEXTURE0, framebuf_.colortexs()[current_texture_]);
  mojgame::gl_shader::set_uniform_i(blit_shader_program_, "texture", 0);

  mojgame::glUnbindDrawFramebuffer();

  mojgame::gl_rendering::clear_color_buffer();
  glDrawArrays(GL_QUADS, 0, 4);
}

bool RippleShaderTestSceneGLRenderer::OnRendering(
    const glm::vec2 &window_size) {
  /* Do common set-up */
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  /* Draw on steps */
  RenderOnStep1(window_size);
  RenderOnStep2();

  /* Do common clean-up */
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  return true;
}
