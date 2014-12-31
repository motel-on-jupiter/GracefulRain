/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/GracefulRainGame.h"
#include "core/scene/production/ProductionSceneGraph.h"
#include "core/scene/test/RippleTestScene.h"
#include "core/scene/test/TestSceneGraph.h"
#include "core/scene/GracefulRainScene.h"
#include "core/scene/GracefulRainSceneGraph.h"
#include "mojgame/auxiliary/gl/gl_rendering.h"
#include "mojgame/auxiliary/atb_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/logging/Logger.h"
#include "mojgame/scene/Scene.h"

GracefulRainGame::GracefulRainGame(TwBar &tweak_bar)
    : tweak_bar_(tweak_bar),
      production_graph_(),
      test_graph_(),
      production_graph_iter_(production_graph_),
      test_graph_iter_(test_graph_),
      graph_(&production_graph_),
      graph_iter_(&production_graph_iter_),
      ongoing_(true) {
}

GracefulRainGame::~GracefulRainGame() {
}

bool GracefulRainGame::Initialize(const glm::vec2 &window_size, bool test) {
  if (test) {
    graph_ = &test_graph_;
    graph_iter_ = &test_graph_iter_;
  } else {
    graph_ = &production_graph_;
    graph_iter_ = &production_graph_iter_;
  }
  if (!graph_->SetUp(tweak_bar_)) {
    mojgame::LOGGER().Error("Failed to initialize the test scene graph");
    return false;
  }
  if (!graph_iter_->Next(window_size)) {
    mojgame::LOGGER().Error("Failed to set up the first scene");
    test_graph_.CleanUp();
    return false;
  }
  ongoing_ = true;
  return true;
}

void GracefulRainGame::Finalize() {
  graph_iter_->Clean();
  graph_->CleanUp();
}

bool GracefulRainGame::Step(float elapsed_time, const glm::vec2 &window_size) {
  if (ongoing_) {
    mojgame::BaseScene *scene = graph_iter_->current();
    if (scene != nullptr) {
      if (!scene->Step(elapsed_time)) {
        return false;
      }
      if (scene->finished()) {
        graph_iter_->Next(window_size);
        if (graph_iter_->current() == nullptr) {
          ongoing_ = false;
        }
      }
    }
  }
  return true;
}

bool GracefulRainGame::Render(const glm::vec2 &window_size) {
  if (ongoing_) {
    mojgame::BaseScene *scene = graph_iter_->current();
    if (scene != nullptr) {
      return scene->Render(window_size);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_KeyboardEvent &keyboard) {
  if (ongoing_) {
    GracefulRainBaseScene *scene =
        static_cast<GracefulRainBaseScene *>(graph_iter_->current());
    if (scene != nullptr) {
      return scene->React(keyboard);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseMotionEvent &motion,
                             const glm::vec2 &window_size) {
  if (ongoing_) {
    GracefulRainBaseScene *scene =
        static_cast<GracefulRainBaseScene *>(graph_iter_->current());
    if (scene != nullptr) {
      return scene->React(motion, window_size);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseButtonEvent &button,
                             const glm::vec2 &window_size) {
  if (ongoing_) {
    GracefulRainBaseScene *scene =
        static_cast<GracefulRainBaseScene *>(graph_iter_->current());
    if (scene != nullptr) {
      return scene->React(button, window_size);
    }
  }
  return true;
}

bool GracefulRainGame::React(const SDL_MouseWheelEvent &wheel,
                             const glm::vec2 &window_size) {
  if (ongoing_) {
    GracefulRainBaseScene *scene =
        static_cast<GracefulRainBaseScene *>(graph_iter_->current());
    if (scene != nullptr) {
      return scene->React(wheel, window_size);
    }
  }
  return true;
}
