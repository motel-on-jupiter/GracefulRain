/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CORE_SCENE_TEST_TESTSELECTORSCENE_H_
#define CORE_SCENE_TEST_TESTSELECTORSCENE_H_

#include <vector>
#include "core/scene/GracefulRainScene.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/scene/SceneGraph.h"

class TestSelectorScene : public GracefulRainBaseScene,
    public mojgame::NonCopyable<TestSelectorScene> {
 public:
  static const std::string kName;

  TestSelectorScene(mojgame::SceneGraph &graph, TwBar &tweak_bar)
      : GracefulRainBaseScene(kName.c_str(), tweak_bar),
        graph_(graph),
        scenes_(),
        cursor_(0),
        escaped_(false) {
  }
  const std::vector<mojgame::BaseScene *> &scenes() const {
    return scenes_;
  }
  unsigned int cursor() const {
    return cursor_;
  }
  bool escaped() const {
    return escaped_;
  }

 protected:
  virtual bool OnInitial(const glm::vec2 &window_size);
  virtual void OnFinal();
  virtual bool OnStep(float elapsed_time);
  virtual bool OnRendering(const glm::vec2 &window_size);
  virtual bool OnReaction(const SDL_KeyboardEvent &keyboard);

 private:
  mojgame::SceneGraph &graph_;
  std::vector<mojgame::BaseScene *> scenes_;
  unsigned int cursor_;
  bool escaped_;

};

#endif /* CORE_SCENE_TEST_TESTSELECTORSCENE_H_ */
