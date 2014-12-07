/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/GracefulRainGame.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/atb_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/logging/Logger.h"

GracefulRainGame::GracefulRainGame()
    : scenes_(),
      current_scene_(nullptr),
      cursor_(0),
      ongoing_(false) {
}

GracefulRainGame::~GracefulRainGame() {
}

int GracefulRainGame::Initialize() {
  ongoing_ = true;
  return 0;
}

void GracefulRainGame::Finalize(TwBar &tweak_bar) {
  if (current_scene_ != nullptr) {
    current_scene_->Finalize(tweak_bar);
    current_scene_ = nullptr;
  }
  for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {
    delete *it;
  }
  scenes_.clear();
  ongoing_ = false;
}

void GracefulRainGame::Update(float elapsed_time, const glm::vec2 &window_size) {
  if (!ongoing_) {
    return;
  }
  if (current_scene_ != nullptr) {
    current_scene_->Update(elapsed_time, window_size);
  }
}

void GracefulRainGame::Draw(const glm::vec2 &window_size) {
  if (!ongoing_) {
    return;
  }

  mojgame::gl_rendering::clear_color_buffer();
  mojgame::gl_rendering::clear_depth_buffer();

  if (current_scene_ == nullptr) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(
        glm::value_ptr(glm::ortho(0.0f, window_size.x, window_size.y, 0.0f)));

    glMatrixMode(GL_MODELVIEW);
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
  } else {
    current_scene_->Draw(window_size);
  }
}

bool GracefulRainGame::React(const SDL_KeyboardEvent &keyboard,
                       const glm::vec2 &window_size, TwBar &tweak_bar) {
  if (!ongoing_) {
    return true;
  }
  if (current_scene_ == nullptr) {
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
          GracefulRainBaseGameScene *setup_scene = scenes_.at(cursor_);
          mojgame::LOGGER().Info("Set up the game scene (scene: %s)",
                                 setup_scene->name().c_str());
          int ret = setup_scene->Initialize(window_size, tweak_bar);
          if (ret < 0) {
            mojgame::LOGGER().Error(
                "Failed to setup the scene (ret: %d, scene: %s)", ret,
                setup_scene->name().c_str());
            return false;
          }
          current_scene_ = setup_scene;
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
        current_scene_->Finalize(tweak_bar);
        current_scene_ = nullptr;
      }
    } else {
      current_scene_->React(keyboard);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseMotionEvent &motion,
                       const glm::vec2 &window_size) {
  UNUSED(window_size);

  if (ongoing_) {
    if (current_scene_ != nullptr) {
      current_scene_->React(motion);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseButtonEvent &button,
                       const glm::vec2 &window_size) {
  UNUSED(window_size);

  if (ongoing_) {
    if (current_scene_ != nullptr) {
      current_scene_->React(button);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseWheelEvent &wheel,
                       const glm::vec2 &window_size) {
  UNUSED(window_size);

  if (ongoing_) {
    if (current_scene_ != nullptr) {
      current_scene_->React(wheel);
    }
  }
  return true;
}
