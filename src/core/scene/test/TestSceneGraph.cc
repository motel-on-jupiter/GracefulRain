/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/test/TestSceneGraph.h"
#include "core/scene/test/RippleTestScene.h"
#include "core/scene/test/TestSelectorScene.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneGraph.h"

bool TestSceneGraph::SetUp(TwBar &tweak_bar) {
  {
    mojgame::BaseScene *sub_scene = new TestSelectorScene(*this, tweak_bar);
    if (sub_scene == nullptr) {
      mojgame::LOGGER().Error("Failed to allocate for selector scene object");
      CleanUp();
      return false;
    }
    pool_[TestSelectorScene::kName] = sub_scene;
  }
  {
    mojgame::BaseScene *sub_scene = new RippleTestScene(tweak_bar);
    if (sub_scene == nullptr) {
      mojgame::LOGGER().Error(
          "Failed to allocate for ripple test scene object");
      CleanUp();
      return false;
    }
    pool_[RippleTestScene::kName] = sub_scene;
  }

  InsertAsRoot(pool_[TestSelectorScene::kName]);
  Insert(pool_[TestSelectorScene::kName], pool_[RippleTestScene::kName]);
  InsertAsLeaf(pool_[TestSelectorScene::kName]);
  InsertAsLeaf(pool_[RippleTestScene::kName]);
  return true;
}

void TestSceneGraph::CleanUp() {
  Clear();
  for (auto it = pool_.begin(); it != pool_.end(); ++it) {
    delete it->second;
  }
  pool_.clear();
}

bool TestSceneGraph::OnChoice(mojgame::BaseScene *current,
                              Scenes::iterator candidates_begin,
                              Scenes::iterator candidates_end,
                              Scenes::iterator &next) const {
  if (current == pool_.at(TestSelectorScene::kName)) {
    TestSelectorScene *selector_scene = static_cast<TestSelectorScene *>(pool_
        .at(TestSelectorScene::kName));
    mojgame::BaseScene *next_scene = nullptr;
    if (!selector_scene->escaped()) {
      const std::vector<mojgame::BaseScene *> &scenes =
          selector_scene->scenes();
      unsigned int cursor = selector_scene->cursor();
      if (cursor >= scenes.size()) {
        mojgame::LOGGER().Error(
            "Invalid cursor value (cursor: %u, num_scenes: %u)", cursor,
            scenes.size());
        return false;
      }
      next_scene = scenes[cursor];
    }
    auto it = candidates_begin;
    for (; it != candidates_end; ++it) {
      if (*it == next_scene) {
        next = it;
        break;
      }
    }
    if (it == candidates_end) {
      mojgame::LOGGER().Error("Choiced scene doesn't exists in candidates");
      return false;
    }
  } else {
    assert_not_reached();
  }
  return true;
}
