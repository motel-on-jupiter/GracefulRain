/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CORE_SCENE_RIPPLESHADERTESTSCENE_H_
#define CORE_SCENE_RIPPLESHADERTESTSCENE_H_

#include "core/GracefulRainGame.h"
#include "mojgame/catalogue/renderer/gl/GradationalGLRenderer.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/scene/Scene.h"

class RippleShaderTestScene : public GracefulRainBaseScene {
 public:
  explicit RippleShaderTestScene(TwBar &tweak_bar);
  virtual ~RippleShaderTestScene() {
  }

  const glm::vec2 &dragging_pos() const {
    return dragging_pos_;
  }

 protected:
  virtual bool OnInitial();
  virtual void OnFinal();
  virtual bool OnStep(float elapsed_time);
  virtual bool OnReaction(const SDL_MouseButtonEvent &button,
                          const glm::vec2 &window_size);
  virtual bool OnReaction(const SDL_MouseMotionEvent &motion,
                          const glm::vec2 &window_size);

 private:
  glm::vec2 dragging_pos_;
};

class RippleShaderTestSceneGLRenderer : public mojgame::BaseSceneRenderer {
 public:
  explicit RippleShaderTestSceneGLRenderer(RippleShaderTestScene &scene);
  virtual ~RippleShaderTestSceneGLRenderer() {
  }

  RippleShaderTestScene &GetScene() {
    return *static_cast<RippleShaderTestScene *>(&scene());
  }

 protected:
  virtual bool OnInitial(const glm::vec2 &window_size);
  virtual void OnFinal();
  virtual bool OnRendering(const glm::vec2 &window_size);

 private:
  mojgame::GradationalGLRenderer gradational_renderer_;
  glm::vec2 drop_pos_;
};

#endif /* CORE_SCENE_RIPPLESHADERTESTSCENE_H_ */
