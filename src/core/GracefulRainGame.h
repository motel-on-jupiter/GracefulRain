/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CORE_GRACEFULRAINGAME_H_
#define CORE_GRACEFULRAINGAME_H_

#include "core/scene/production/ProductionSceneGraph.h"
#include "core/scene/test/TestSceneGraph.h"
#include "core/scene/GracefulRainSceneGraph.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneGraph.h"

class GracefulRainGame : public mojgame::NonCopyable<GracefulRainGame> {
 public:
  explicit GracefulRainGame(TwBar &tweak_bar);
  ~GracefulRainGame();

  bool Initialize(const glm::vec2 &window_size, bool test=false);
  void Finalize();
  bool Step(float elapsed_time, const glm::vec2 &window_size);
  bool Render(const glm::vec2 &window_size);
  bool React(const SDL_KeyboardEvent &keyboard);
  bool React(const SDL_MouseMotionEvent &motion, const glm::vec2 &window_size);
  bool React(const SDL_MouseButtonEvent &button, const glm::vec2 &window_size);
  bool React(const SDL_MouseWheelEvent &wheel, const glm::vec2 &window_size);

  bool ongoing() const {
    return ongoing_;
  }

 private:
  TwBar &tweak_bar_;
  ProductionSceneGraph production_graph_;
  TestSceneGraph test_graph_;
  mojgame::SceneGraphIterator production_graph_iter_;
  mojgame::SceneGraphIterator test_graph_iter_;
  GracefulRainSceneGraph *graph_;
  mojgame::SceneGraphIterator *graph_iter_;
  bool ongoing_;
};

#endif /* CORE_GRACEFULRAINGAME_H_ */
