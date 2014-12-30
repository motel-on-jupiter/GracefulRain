/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/production/ProductionScene.h"
#include "core/scene/production/ProductionSceneGraph.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"
#include "mojgame/scene/SceneGraph.h"

bool ProductionSceneGraph::SetUp(TwBar &tweak_bar) {
  UNUSED(&tweak_bar);

  mojgame::BaseScene *scene = new ProductionScene(tweak_bar);
  if (scene == nullptr) {
    return false;
  }
  pool_[ProductionScene::kName] = scene;
  InsertAsRoot(pool_[ProductionScene::kName]);
  return true;
}

void ProductionSceneGraph::CleanUp() {
  Clear();
  for (auto it = pool_.begin(); it != pool_.end(); ++it) {
    delete it->second;
  }
  pool_.clear();
}

bool ProductionSceneGraph::OnChoice(mojgame::BaseScene *current,
                                    Scenes::iterator candidates_begin,
                                    Scenes::iterator candidates_end,
                                    Scenes::iterator &next) const {
  UNUSED(current);

  mojgame::BaseScene *next_scene = nullptr;
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
  return true;
}
