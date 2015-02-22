/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "core/scene/production/ProductionScene.h"
#include <string>
#include <vector>
#include "core/GracefulRainTweakerContext.h"
#include "core/scene/GracefulRainScene.h"
#include "mojgame/audio/BgmPlayer.h"
#include "mojgame/auxiliary/coroutine_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/catalogue/renderer/RippleRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/glm_include.h"
#include "mojgame/sampler/color_sample.h"
#include "mojgame/scene/Scene.h"

const std::string ProductionScene::kName("Production");

const glm::vec2 ProductionScene::kPhantomInitialAppearingPoss[] = {
  glm::vec2(0.45f, 0.0f),
  glm::vec2(0.55f, 0.0f),
  glm::vec2(0.45f, 1.0f),
  glm::vec2(0.55f, 1.0f),
};

const glm::vec3 ProductionScene::kRainStimulusColor(1.0f);
const glm::vec2 ProductionScene::kRainStimulusEffectRange(0.3f, 0.7f);

const glm::vec3 ProductionScene::kMouseStimulusColor(0.0f, 1.0f, 0.5f);
const float ProductionScene::kMouseStimulusEffect = 1.0f;

const float ProductionScene::kBattleTime = 120.0f;
const float ProductionScene::kBattleTimeToStartSeriousBattle = 30.0f;
const float ProductionScene::kBattleTimeToStartHardBattle = 60.0f;
const float ProductionScene::kMaxEscapeDistance = 0.15f;

ProductionScene::ProductionScene(TwBar &tweak_bar)
    : GracefulRainBaseScene(kName.c_str(), tweak_bar),
      ripple_renderer_(),
      rainy_stimulator_(kRainStimulusColor, kRainStimulusEffectRange),
      telop_renderer_(),
      renderer_stack_(),
      rain_bgm_("audio/rain.wav"),
      thunder_bgm_("audio/thunder.wav"),
      forest_bgm_("audio/forest.wav"),
      footstep_se_("audio/footstep_place.wav"),
      phantom_voice_se_("audio/phantom_voice.wav"),
      meteo_se_("audio/meteo.wav"),
      ccr_param_(nullptr),
      rina_(),
      pablo_(),
      phantoms_(),
      stimulus_(),
      timer_(0.0f),
      rina_escape_timer_(0.0f),
      thunder_bgm_playing_(false) {
}

bool ProductionScene::OnInitial(const glm::vec2 &window_size) {
  if (!ripple_renderer_.Initialize(window_size)) {
    mojgame::LOGGER().Error("Failed to initialize ripple renderer");
    return false;
  }
  ripple_renderer_.Attach(rainy_stimulator_);
  if (!telop_renderer_.Initialize()) {
    mojgame::LOGGER().Error("Failed to initialize telop renderer");
    ripple_renderer_.Finalize();
    return false;
  }
  if (!rain_bgm_.Initialize(1.0f, 0.3f)) {
    mojgame::LOGGER().Error("Failed to initialize rain ambient bgm");
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  if (!thunder_bgm_.Initialize(1.0f, 1.0f)) {
    mojgame::LOGGER().Error("Failed to initialize thunder ambient bgm");
    rain_bgm_.Finalize();
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  if (!forest_bgm_.Initialize(1.0f, 0.3f)) {
    mojgame::LOGGER().Error("Failed to initialize forest ambient bgm");
    thunder_bgm_.Finalize();
    rain_bgm_.Finalize();
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  if (!footstep_se_.Initialize(1.0f, 0.1f)) {
    mojgame::LOGGER().Error("Failed to initialize footstep se");
    forest_bgm_.Finalize();
    thunder_bgm_.Finalize();
    rain_bgm_.Finalize();
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  if (!phantom_voice_se_.Initialize(1.0f, 0.5f)) {
    mojgame::LOGGER().Error("Failed to initialize phantom voice se");
    footstep_se_.Finalize();
    forest_bgm_.Finalize();
    thunder_bgm_.Finalize();
    rain_bgm_.Finalize();
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  if (!meteo_se_.Initialize(1.0f, 0.2f)) {
    mojgame::LOGGER().Error("Failed to initialize meteo se");
    phantom_voice_se_.Finalize();
    footstep_se_.Finalize();
    forest_bgm_.Finalize();
    thunder_bgm_.Finalize();
    rain_bgm_.Finalize();
    telop_renderer_.Finalize();
    ripple_renderer_.Finalize();
    return false;
  }
  rina_.AttachFootstepSe(footstep_se_);
  pablo_.AttachFootstepSe(footstep_se_);
  mojgame::atb_aux::AddColor3fVarRW(tweak_bar(), "Ripples", "Color Filter",
                                    tweaker_ctx.ripples_rgb_filter, nullptr);
  alListenerfv(AL_POSITION, glm::value_ptr(glm::vec3()));
  return true;
}

void ProductionScene::OnFinal() {
  ccrAbort(ccr_param_);
  mojgame::atb_aux::RemoveVar(tweak_bar(), "Ripples", "Color Filter");
  rina_.DettachFootstepSe();
  pablo_.DettachFootstepSe();
  meteo_se_.Finalize();
  phantom_voice_se_.Finalize();
  footstep_se_.Finalize();
  forest_bgm_.Finalize();
  thunder_bgm_.Finalize();
  rain_bgm_.Finalize();
  renderer_stack_.clear();
  telop_renderer_.Finalize();
  ripple_renderer_.DettachAll();
  ripple_renderer_.Finalize();
}

void ProductionScene::PlayPhantomVoice(int phantom) {
  phantom_voice_se_.ChangePitch(phantoms_[phantom].voice_pitch());
  glm::vec3 playing_pos(phantoms_[phantom].pos().y, 0.0f,
                        phantoms_[phantom].pos().y);
  mojgame::AlureSePlayer::Play(phantom_voice_se_, playing_pos);
}

void ProductionScene::RandomizeAppearingPositionForPhantom(glm::vec2 &appearing_pos) {
  float end = glm::linearRand(0.0f, 4.0f);
  if (end < 1.0f) {
    appearing_pos.x = 0.0f;
    appearing_pos.y = glm::linearRand(0.0f, 1.0f);
  } else if (end < 2.0f) {
    appearing_pos.x = 1.0f;
    appearing_pos.y = glm::linearRand(0.0f, 1.0f);
  } else if (end  < 3.0f) {
    appearing_pos.x = glm::linearRand(0.0f, 1.0f);
    appearing_pos.y = 1.0f;
  } else {
    appearing_pos.x = glm::linearRand(0.0f, 1.0f);
    appearing_pos.y = 0.0f;
  }
}

void ProductionScene::Direct() {
  int i = 0;
  int j = 0;
  glm::vec2 escape_pos;
  ccrAsContParam(ccr_param_);
  ccrBeginContext;
  ccrEndContext(ctx);
  ccrBegin_(ctx);
  if (!mojgame::AlureBgmPlayer::Play(rain_bgm_, -1)) {
    mojgame::LOGGER().Error("Failed to play rain ambient bgm");
  }
  renderer_stack_.push_back(&ripple_renderer_);
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Developed by The Motel on Jupiter", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  timer_ = 0.0f;
  while (timer_ < 4.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  timer_ = 0.0f;
  while (timer_ < 0.1f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("- Lost in Rain -", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  timer_ = 0.0f;
  while (timer_ < 4.0f) {
    ccrReturnV;
  }
  renderer_stack_.pop_back();
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  rina_.Appear(glm::vec2(0.5f, 0.0f));
  rina_.Walk(glm::vec2(0.5f, 0.5f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 0.5f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 1.0f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 0.5f) {
    ccrReturnV;
  }
  pablo_.Appear(glm::vec2(0.5f, 0.0f));
  pablo_.Walk(glm::vec2(0.5f, 0.5f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  pablo_.Walk(glm::vec2(0.4f, 0.5f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  pablo_.Walk(glm::vec2(0.6f, 0.5f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  pablo_.Walk(glm::vec2(0.6f, 1.0f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Appear(glm::vec2(0.5f, 1.0f));
  rina_.Walk(glm::vec2(0.5f, 0.4f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  for (i = 0; i < 2; ++i) {
    phantoms_[i].Appear(kPhantomInitialAppearingPoss[i]);
    phantoms_[i].Walk(rina_);
    PlayPhantomVoice(i);
  }
  rina_.Walk(glm::vec2(0.5f, 0.6f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  for (i = 2; i < 4; ++i) {
    phantoms_[i].Appear(kPhantomInitialAppearingPoss[i]);
    phantoms_[i].Walk(rina_);
    PlayPhantomVoice(i);
  }
  rina_.Walk(glm::vec2(0.5f, 0.5f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  rina_escape_timer_ = 0.0f;
  for (i = 4; i < ARRAYSIZE(phantoms_); ++i) {
    glm::vec2 appearing_pos;
    RandomizeAppearingPositionForPhantom(appearing_pos);
    phantoms_[i].Appear(appearing_pos);
    phantoms_[i].Walk(rina_);
  }
  while (timer_ < kBattleTime) {
    if (thunder_bgm_playing_) {
      float thunder_time = timer_ - kBattleTimeToStartHardBattle;
      while (thunder_time > 30.0f) {
        thunder_time -= 30.0f;
      }
      if (thunder_time > 0.95f && thunder_time < 1.35f) {
        ripple_renderer_.set_filling_level(0.9f);
      } else if (thunder_time > 1.35f && thunder_time < 1.40f) {
        ripple_renderer_.set_filling_level((1.0f - mojgame::math_aux::square((thunder_time - 1.35f) / 0.05f)) * 0.9f);
      } else {
        ripple_renderer_.set_filling_level(0.0f);
      }
    } else {
      if (timer_ >= kBattleTimeToStartHardBattle) {
        if (!mojgame::AlureBgmPlayer::Play(thunder_bgm_, -1)) {
          mojgame::LOGGER().Error("Failed to play thunder ambient bgm");
        }
        thunder_bgm_playing_ = true;
      }
    }
    if (stimulus_.effect > 0.0f) {
      for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
        if (glm::length2(phantoms_[i].pos() - stimulus_.pos) < 0.1f * 0.1f) {
          phantoms_[i].ReceiveDamage();
          if (phantoms_[i].IsDead()) {
            glm::vec2 appearing_pos;
            PlayPhantomVoice(i);
            RandomizeAppearingPositionForPhantom(appearing_pos);
            phantoms_[i].Revive(timer_ >= kBattleTimeToStartHardBattle,
                                (timer_ >= kBattleTimeToStartSeriousBattle) ?
                                    glm::linearRand(0.8f, 2.0f) :
                                    glm::linearRand(0.8f, 1.2f),
                                appearing_pos);
          }
        }
      }
    }
    for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
      escape_pos += phantoms_[i].pos() / static_cast<float>(ARRAYSIZE(phantoms_));
      if (glm::distance(escape_pos, glm::vec2(0.5f)) > kMaxEscapeDistance) {
        escape_pos = glm::vec2(0.5f) + (escape_pos - glm::vec2(0.5f)) * kMaxEscapeDistance;
      }
    }
    if (rina_.IsWalking()) {
      rina_escape_timer_ = 0.0f;
    } else {
      if (rina_escape_timer_ > 2.5f) {
        rina_escape_timer_ = 0.0f;
        rina_.Walk(escape_pos);
      }
    }
    for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
      if (!phantoms_[i].IsWalking()) {
        for (j = 0; j<ARRAYSIZE(phantoms_); ++j) {
          phantoms_[j].Stop();
        }
        telop_renderer_.Reset("Game Over", glm::vec2(0.5f));
        renderer_stack_.push_back(&telop_renderer_);
        while (true) {
          ccrReturnV;
        }
      }
    }
    ccrReturnV;
  }
  for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    phantoms_[i].Stop();
    PlayPhantomVoice(i);
  }
  timer_ = 0.0f;
  while (timer_ < 3.0f) {
    thunder_bgm_.ChangeGain(1.0f * (3.0f - timer_));
    ccrReturnV;
  }
  if (!mojgame::AlureBgmPlayer::Stop(thunder_bgm_)) {
    mojgame::LOGGER().Error("Failed to stop thunder ambient bgm");
  }
  rina_.Walk(glm::vec2(0.5f, 0.4f));
  timer_ = 0.0f;
  while (timer_ < 5.0f) {
    rainy_stimulator_.set_effect_range(kRainStimulusEffectRange * (1.0f - timer_ / 5.0f));
    rain_bgm_.ChangeGain(0.3f * (1.0f - timer_ / 5.0f));
    ccrReturnV;
  }
  rainy_stimulator_.set_effect_range(kRainStimulusEffectRange * 0.0f);
  for (i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    phantoms_[i].Disappear();
  }
  ripple_renderer_.DettachAll();
  if (!mojgame::AlureBgmPlayer::Stop(rain_bgm_)) {
    mojgame::LOGGER().Error("Failed to stop rain ambient bgm");
  }
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    forest_bgm_.ChangeGain(0.3f * (timer_ / 2.0f));
    ccrReturnV;
  }
  if (!mojgame::AlureBgmPlayer::Play(forest_bgm_, -1)) {
    mojgame::LOGGER().Error("Failed to play forest ambient bgm");
  }
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  pablo_.Appear(glm::vec2(0.55f, 1.0f));
  pablo_.Walk(glm::vec2(0.55f, 0.55f));
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 0.5f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 0.5f));
  while (rina_.IsWalking() || pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.0f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 0.03f));
  pablo_.Walk(glm::vec2(0.55f, 0.2f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  pablo_.Walk(glm::vec2(0.55f, 0.425f));
  while (pablo_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 3.0f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 0.3f));
  while (rina_.IsWalking()) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 0.5f) {
    ccrReturnV;
  }
  rina_.Hop();
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  rina_.Stamp();
  timer_ = 0.0f;
  while (timer_ < 0.25f) {
    ccrReturnV;
  }
  rina_.Stamp();
  timer_ = 0.0f;
  while (timer_ < 0.25f) {
    ccrReturnV;
  }
  rina_.Stamp();
  timer_ = 0.0f;
  while (timer_ < 0.25f) {
    ccrReturnV;
  }
  rina_.Stamp();
  timer_ = 0.0f;
  while (timer_ < 0.25f) {
    ccrReturnV;
  }
  rina_.Stamp();
  timer_ = 0.0f;
  while (timer_ < 2.0f) {
    ccrReturnV;
  }
  pablo_.Walk(glm::vec2(0.55f, 0.0f));
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  rina_.Walk(glm::vec2(0.5f, 0.0f));
  timer_ = 0.0f;
  while (timer_ < 1.5f) {
    ccrReturnV;
  }
  telop_renderer_.Reset("Fin", glm::vec2(0.5f));
  renderer_stack_.push_back(&telop_renderer_);
  timer_ = 0.0f;
  while (timer_ < 30.0f) {
    ccrReturnV;
  }
  timer_ = 0.0f;
  while (timer_ < 10.0f) {
    forest_bgm_.ChangeGain(0.3f * (1.0f - timer_ / 10.0f));
    ccrReturnV;
  }
  if (!mojgame::AlureBgmPlayer::Stop(forest_bgm_)) {
    mojgame::LOGGER().Error("Failed to stop forest ambient bgm");
  }
  while (true) {
    ccrReturnV;
  }
  ccrFinishV;
}

bool ProductionScene::OnStep(float elapsed_time) {
  timer_ += elapsed_time;
  rina_escape_timer_ += elapsed_time;
  if (!finished()) {
    Direct();
  }
  if (!rina_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Rina");
    return false;
  }
  if (!rina_.Stimulate(ripple_renderer_)) {
    mojgame::LOGGER().Error("Failed for Rina to stimulate");
    return false;
  }
  if (!pablo_.Step(elapsed_time)) {
    mojgame::LOGGER().Error("Failed to step Pablo");
    return false;
  }
  if (!pablo_.Stimulate(ripple_renderer_)) {
    mojgame::LOGGER().Error("Failed for Pablo to stimulate");
    return false;
  }
  for (int i = 0; i<ARRAYSIZE(phantoms_); ++i) {
    if (!phantoms_[i].Step(elapsed_time)) {
      mojgame::LOGGER().Error("Failed to step Phantom (idx: %d)", i);
      return false;
    }
    if (!phantoms_[i].Stimulate(ripple_renderer_)) {
      mojgame::LOGGER().Error("Failed for Phantom to stimulate (idx: %d)", i);
      return false;
    }
  }
  if (stimulus_.effect > 0.0f) {
    ripple_renderer_.Receive(stimulus_);
    stimulus_.effect = 0.0f;
  }
  return true;
}

bool ProductionScene::OnRendering(const glm::vec2 &window_size) {
  if (ripple_renderer_.filtering_r() != tweaker_ctx.ripples_rgb_filter.r) {
    ripple_renderer_.set_filtering_r(tweaker_ctx.ripples_rgb_filter.r);
  }
  if (ripple_renderer_.filtering_g() != tweaker_ctx.ripples_rgb_filter.g) {
    ripple_renderer_.set_filtering_g(tweaker_ctx.ripples_rgb_filter.g);
  }
  if (ripple_renderer_.filtering_b() != tweaker_ctx.ripples_rgb_filter.b) {
    ripple_renderer_.set_filtering_b(tweaker_ctx.ripples_rgb_filter.b);
  }
  for (auto it = renderer_stack_.begin(); it != renderer_stack_.end(); ++it) {
    if (!(*it)->Render(window_size)) {
      mojgame::LOGGER().Warn("Failed to render by renderer");
    }
  }
  return true;
}

void ProductionScene::ActuateMeteo(const glm::vec2 &pos) {
  stimulus_.pos = pos + glm::diskRand(0.1f);
  stimulus_.color = kMouseStimulusColor;
  stimulus_.effect = kMouseStimulusEffect;
  glm::vec3 playing_pos(stimulus_.pos.x, 0.0f, stimulus_.pos.y);
  mojgame::AlureSePlayer::Play(meteo_se_, playing_pos);
}

bool ProductionScene::OnReaction(const SDL_MouseButtonEvent &button,
                                 const glm::vec2 &window_size) {
  if (button.button == 1) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
      glm::vec2 pos = glm::vec2(static_cast<float>(button.x),
                                static_cast<float>(button.y)) / window_size;
      pos.y = 1.0f - pos.y;
      ActuateMeteo(pos);
    }
  }
  return true;
}

bool ProductionScene::OnReaction(const SDL_MouseMotionEvent &motion,
                                 const glm::vec2 &window_size) {
  if (motion.state == SDL_PRESSED) {
    glm::vec2 pos = glm::vec2(static_cast<float>(motion.x),
                              static_cast<float>(motion.y)) / window_size;
    pos.y = 1.0f - pos.y;
    ActuateMeteo(pos);
  }
  return true;
}
