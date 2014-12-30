/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/scene/test/TestSelectorScene.h"
#include "mojgame/auxiliary/csyntax_aux.h"

const std::string TestSelectorScene::kName("Test Selector");

bool TestSelectorScene::OnInitial(const glm::vec2 &window_size) {
  UNUSED(window_size);

  graph_.Trace(this, scenes_, true);
  cursor_ = 0;
  escaped_ = false;
  return true;
}

void TestSelectorScene::OnFinal() {
  scenes_.clear();
}

bool TestSelectorScene::OnStep(float elapsed_time) {
  UNUSED(elapsed_time);
  return true;
}

bool TestSelectorScene::OnRendering(const glm::vec2 &window_size) {
  mojgame::gl_rendering::clear_color_buffer();
  mojgame::gl_rendering::clear_depth_buffer();

  glMatrixMode (GL_PROJECTION);
  glLoadMatrixf(
      glm::value_ptr(glm::ortho(0.0f, window_size.x, window_size.y, 0.0f)));

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec2 name_pos = glm::vec2(10.0f, 20.0f);
  unsigned int idx = 0;
  for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {
    glRasterPos2fv(glm::value_ptr(name_pos));
    glutBitmapString(
        GLUT_BITMAP_9_BY_15,
        reinterpret_cast<const unsigned char *>((
            (idx == cursor_) ? "-> " : "   ")));
    glutBitmapString(
        GLUT_BITMAP_9_BY_15,
        reinterpret_cast<const unsigned char *>((*it)->name().c_str()));
    name_pos += glm::vec2(0.0f, 12);
    ++idx;
  }
  return true;
}

bool TestSelectorScene::OnReaction(const SDL_KeyboardEvent &keyboard) {
  if (keyboard.state == SDL_PRESSED) {
    switch (keyboard.keysym.sym) {
      case SDLK_j:
      case SDLK_DOWN:
        if (cursor_ < scenes_.size() - 1) {
          ++cursor_;
        }
        break;
      case SDLK_k:
      case SDLK_UP:
        if (cursor_ > 0) {
          --cursor_;
        }
        break;
      case SDLK_RETURN: {
        if (cursor_ < scenes_.size()) {
          Finish();
        }
        break;
      }
      case SDLK_ESCAPE:
        escaped_ = true;
        Finish();
        break;
    }
  }
  return true;
}
