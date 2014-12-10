/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/GracefulRainGame.h"
#include "core/scene/RippleShaderTestScene.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/atb_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/logging/Logger.h"
#include "mojgame/scene/Scene.h"

GracefulRainGame::GracefulRainGame(TwBar &tweak_bar)
    : scene_suites_(),
      tweak_bar_(tweak_bar),
      ongoing_(false),
      current_(-1),
      cursor_(0) {
}

GracefulRainGame::~GracefulRainGame() {
}

void GracefulRainGame::Initialize() {
  RippleShaderTestScene *main_scene = new RippleShaderTestScene(tweak_bar_);
  mojgame::BaseSceneRenderer *renderer = new RippleShaderTestSceneGLRenderer(*main_scene);
  GracefulRainSceneSuite *suite = new GracefulRainSceneSuite(*main_scene, *renderer);
  scene_suites_.push_back(suite);
  ongoing_ = true;
}

void GracefulRainGame::Finalize() {
  if (current_ != -1) {
    scene_suites_[current_]->Finalize();
    current_ = -1;
  }
  for (auto it = scene_suites_.begin(); it != scene_suites_.end(); ++it) {
    delete &((*it)->scene());
    delete &((*it)->renderer());
  }
  scene_suites_.clear();
  ongoing_ = false;
}

bool GracefulRainGame::Step(float elapsed_time) {
  if (ongoing_ && current_ != -1) {
    return scene_suites_[current_]->Step(elapsed_time);
  }
  return true;
}

bool GracefulRainGame::Render(const glm::vec2 &window_size) {
  if (ongoing_) {
    mojgame::gl_rendering::clear_color_buffer();
    mojgame::gl_rendering::clear_depth_buffer();

    if (current_ == -1) {
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf(
          glm::value_ptr(glm::ortho(0.0f, window_size.x, window_size.y, 0.0f)));

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glm::vec2 name_pos = glm::vec2(10.0f, 20.0f);
      unsigned int idx = 0;
      for (auto it = scene_suites_.begin(); it != scene_suites_.end(); ++it) {
        glRasterPos2fv(glm::value_ptr(name_pos));
        glutBitmapString(
            GLUT_BITMAP_9_BY_15,
            reinterpret_cast<const unsigned char *>((
                (idx == cursor_) ? "-> " : "   ")));
        glutBitmapString(
            GLUT_BITMAP_9_BY_15,
            reinterpret_cast<const unsigned char *>((*it)->scene().name().c_str()));
        name_pos += glm::vec2(0.0f, 12);
        ++idx;
      }
    } else {
      return scene_suites_[current_]->Render(window_size);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_KeyboardEvent &keyboard, const glm::vec2 &window_size) {
  if (!ongoing_) {
    return true;
  }
  if (current_ == -1) {
    if (keyboard.state == SDL_PRESSED) {
      switch (keyboard.keysym.sym) {
        case SDLK_j:
        case SDLK_DOWN:
          if (cursor_ < scene_suites_.size() - 1) {
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
          if (cursor_ < scene_suites_.size()) {
            mojgame::LOGGER().Info("Set up the game scene (scene: %s)",
                                   scene_suites_[cursor_]->scene().name().c_str());
            if (!scene_suites_[cursor_]->Initialize(window_size)) {
              mojgame::LOGGER().Error(
                  "Failed to setup the scene (scene: %s)",
                  scene_suites_[cursor_]->scene().name().c_str());
              return false;
            }
            current_ = cursor_;
          }
          break;
        }
        case SDLK_ESCAPE:
          ongoing_ = false;
          break;
      }
    }
  } else {
    if (keyboard.keysym.sym == SDLK_ESCAPE) {
      if (keyboard.state == SDL_PRESSED) {
        mojgame::LOGGER().Info("Clean up the current scene");
        scene_suites_[current_]->Finalize();
        current_ = -1;
      }
    } else {
      return scene_suites_[current_]->GetScene().React(keyboard);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseMotionEvent &motion,
                             const glm::vec2 &window_size) {
  if (ongoing_ && current_ != -1) {
    return scene_suites_[current_]->GetScene().React(motion, window_size);
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseButtonEvent &button,
                             const glm::vec2 &window_size) {
  if (ongoing_ && current_ != -1) {
    return scene_suites_[current_]->GetScene().React(button, window_size);
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseWheelEvent &wheel,
                             const glm::vec2 &window_size) {
  if (ongoing_ && current_ != -1) {
    return scene_suites_[current_]->GetScene().React(wheel, window_size);
  }
  return true;
}
