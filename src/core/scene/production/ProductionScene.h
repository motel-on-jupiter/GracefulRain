/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_PRODUCTION_PRODUCTIONSCENE_H_
#define CORE_SCENE_PRODUCTION_PRODUCTIONSCENE_H_

#include <string>
#include <vector>
#include "core/actor/Pablo.h"
#include "core/actor/Phantom.h"
#include "core/actor/Rina.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/audio/BgmPlayer.h"
#include "mojgame/auxiliary/coroutine_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/catalogue/renderer/TelopRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"

class ProductionScene : public GracefulRainBaseScene {
 public:
  static const std::string kName;

  explicit ProductionScene(TwBar &tweak_bar);
  virtual ~ProductionScene() {
  }

 protected:
  bool OnInitial(const glm::vec2 &window_size);
  void OnFinal();
  bool OnStep(float elapsed_time);
  bool OnRendering(const glm::vec2 &window_size);
  bool OnReaction(const SDL_MouseButtonEvent &button,
                  const glm::vec2 &window_size);
  bool OnReaction(const SDL_MouseMotionEvent &motion,
                  const glm::vec2 &window_size);

 private:
  enum { kNumPhantoms = 8 };
  static const glm::vec2 kPhantomInitialAppearingPoss[4];
  static const glm::vec3 kRainStimulusColor;
  static const glm::vec2 kRainStimulusEffectRange;
  static const glm::vec3 kMouseStimulusColor;
  static const float kMouseStimulusEffect;
  static const float kBattleTime;
  static const float kBattleTimeToStartSeriousBattle;
  static const float kBattleTimeToStartHardBattle;
  static const float kMaxEscapeDistance;

  void PlayPhantomVoice(int phantom, float gain);
  void RandomizeAppearingPositionForPhantom(glm::vec2 &appearing_pos);
  void Direct();
  void ActuateMeteo(const glm::vec2 &pos);

  mojgame::RippleGLRenderer ripple_renderer_;
  mojgame::RainyRippleStimulator rainy_stimulator_;
  mojgame::GLTelopRenderer telop_renderer_;
  std::vector<mojgame::BaseRenderer *> renderer_stack_;
  mojgame::AlureBgm rain_bgm_;
  mojgame::AlureBgm thunder_bgm_;
  mojgame::AlureBgm forest_bgm_;
  mojgame::AlureSe footstep_se_;
  mojgame::AlureSe phantom_voice_se_;
  mojgame::AlureSe meteo_se_;
  void *ccr_param_;
  Rina rina_;
  Pablo pablo_;
  Phantom phantoms_[kNumPhantoms];
  mojgame::RippleStimulus stimulus_;
  float direction_timer_;
  float rina_escape_timer_;
  bool thunder_bgm_playing_;
  int meteo_se_interval_;
};

#endif /* CORE_SCENE_PRODUCTION_PRODUCTIONSCENE_H_ */
