/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_GRACEFULRAINSCENEGRAPH_H_
#define CORE_SCENE_GRACEFULRAINSCENEGRAPH_H_

#include <map>
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneGraph.h"

class GracefulRainSceneGraph : public mojgame::SceneGraph {
 public:
  GracefulRainSceneGraph()
      : mojgame::SceneGraph() {
  }
  virtual ~GracefulRainSceneGraph() {
  }

  virtual bool SetUp(TwBar &tweak_bar) = 0;
  virtual void CleanUp() = 0;
};

#endif /* CORE_SCENE_GRACEFULRAINSCENEGRAPH_H_ */
