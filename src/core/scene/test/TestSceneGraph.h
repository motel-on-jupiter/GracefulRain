/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_TEST_TESTSCENEGRAPH_H_
#define CORE_SCENE_TEST_TESTSCENEGRAPH_H_

#include <map>
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneGraph.h"

class TestSceneGraph : public mojgame::SceneGraph {
 public:
  explicit TestSceneGraph()
      : mojgame::SceneGraph() {
  }
  virtual ~TestSceneGraph() {
  }

  bool SetUp(TwBar &tweak_bar);
  void CleanUp();
  virtual bool OnChoice(mojgame::BaseScene *current,
                        Scenes::iterator candidates_begin,
                        Scenes::iterator candidates_end,
                        Scenes::iterator &next) const;

 private:
  std::map<std::string, mojgame::BaseScene *> pool_;
};

#endif /* CORE_SCENE_TEST_TESTSCENEGRAPH_H_ */
