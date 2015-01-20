/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/GracefulRainApp.h"
#include <algorithm>
#include <mmsystem.h>
#include "core/GracefulRainGame.h"
#include "core/GracefulRainTweakerContext.h"
#include "mojgame/auxiliary/atb_aux.h"
#include "mojgame/auxiliary/csyntax_aux.h"
#include "mojgame/includer/atb_include.h"
#include "mojgame/includer/gl_include.h"
#include "mojgame/includer/sdl_include.h"
#include "mojgame/logging/Logger.h"
#include "mojgame/misc/FPSCounter.h"

const std::string GracefulRainApp::kWindowCaption =
    "GracefulRain - The Motel on Jupiter";
const glm::u32vec2 GracefulRainApp::kWindowPosition = glm::u32vec2(30, 50);
const Uint32 GracefulRainApp::kWindowScale = 72;
const Uint32 GracefulRainApp::kIdealFrameRate = 60;
const int GracefulRainApp::kFPSCountSamplingTime = 1000;
const int GracefulRainApp::kTweakBarWidth = 240;
const unsigned char GracefulRainApp::kTweakBarColor[3] = { 41, 126, 231 };
const bool GracefulRainApp::kTestMode = false;

GracefulRainApp::GracefulRainApp()
    : window_(nullptr),
      context_(nullptr),
      tweak_bar_(nullptr),
      game_(nullptr),
      fps_counter_(kFPSCountSamplingTime) {
}

GracefulRainApp::~GracefulRainApp() {
}

int GracefulRainApp::Run() {
  static const Uint32 kWindowWidth = kWindowScale * 16;
  static const Uint32 kWindowHeight = kWindowScale * 9;
  static const glm::vec2 kWindowSize = glm::vec2(kWindowWidth, kWindowHeight);
  static const int kGameLoopInterval = 1000 / kIdealFrameRate;
  static const float kGameLoopIntervalSec = 1.0f / kIdealFrameRate;
  static const int kTweakBarHeight =
      std::max(static_cast<int>(kWindowHeight) - 20, 0);
  static const glm::u32vec2 kTweakBarPosition(
      std::max(static_cast<int>(kWindowWidth) - kTweakBarWidth - 10, 0), 10);

  mojgame::LOGGER().Info("Set up the application");

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    mojgame::LOGGER().Error(
        "Failed to initialize SDL video system (errmsg: %s)", SDL_GetError());
    return -1;
  }

  // Enable double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Create the window
  window_ = SDL_CreateWindow(kWindowCaption.c_str(), SDL_WINDOWPOS_CENTERED,
  SDL_WINDOWPOS_CENTERED,
                             kWindowWidth, kWindowHeight, SDL_WINDOW_OPENGL);
  if (window_ == nullptr) {
    mojgame::LOGGER().Error("Failed to create SDL window (errmsg: %s)",
                            SDL_GetError());
    CleanUp();
    return -1;
  }

  // Create OpenGL context
  context_ = SDL_GL_CreateContext(window_);
  if (context_ == nullptr) {
    mojgame::LOGGER().Error(
        "Failed to create SDL context for OpenGL (errmsg: %s)", SDL_GetError());
    CleanUp();
    return -1;
  }

  // Initialize the GLUT (for shader support)
  GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    mojgame::LOGGER().Error("Failed to initialize glew library (errmsg: %s)",
                            glewGetErrorString(glew_error));
    CleanUp();
    return -1;
  }

  // Initialize the GLUT (for bitmap font support)
  int glut_argc = 0;
  glutInit(&glut_argc, { });

  // Initialize the tweaker library
  if (TwInit(TW_OPENGL, NULL) == 0) {
    mojgame::LOGGER().Error(
        "Failed to initialize the tweaker library (errmsg: %s)",
        TwGetLastError());
    CleanUp();
    return -1;
  }
  if (TwWindowSize(kWindowWidth, kWindowHeight) == 0) {
    mojgame::LOGGER().Error(
        "Failed to set the window size to tweaker (errmsg: %s)",
        TwGetLastError());
    CleanUp();
    return -1;
  }
  tweak_bar_ = TwNewBar("TweakMenu");
  if (tweak_bar_ == nullptr) {
    mojgame::LOGGER().Error("Failed to create tweak bar (errmsg: %s)",
                            TwGetLastError());
    CleanUp();
    return -1;
  }
  mojgame::atb_aux::DefineMenu(kTweakBarPosition, kTweakBarWidth,
                               kTweakBarHeight, kTweakBarColor, "iconified='true'");
  mojgame::atb_aux::AddInt32VarRO(*tweak_bar_, "System", "Actual Frame Rate",
                                  tweaker_ctx.system_actual_fps);
  mojgame::atb_aux::AddFloatVarRW(*tweak_bar_, "System", "Time Speed",
                                  tweaker_ctx.system_time_speed,
                                  "min='0' max='30' step='0.5'");

  // Initialize the game
  game_ = new GracefulRainGame(*tweak_bar_);
  if (game_ == nullptr) {
    mojgame::LOGGER().Error("Failed to create game object");
    CleanUp();
    return -1;
  }
  if (!game_->Initialize(kWindowSize, kTestMode)) {
    mojgame::LOGGER().Error("Failed to initialize game object");
    CleanUp();
    return -1;
  }

  // Set the minimum timer resolution
  timeBeginPeriod(1);

  // Execute the mainloop
  int loop_stat = 0;
  while (true) {
    int start_tick = SDL_GetTicks();

    // check event
    bool escape_loop = false;
    SDL_Event event;
    while (SDL_PollEvent(&event) == 1) {
      if (mojgame::atb_aux::TwEventSDL20(&event) != 0) {
        continue;
      }
      switch (event.type) {
        case SDL_QUIT:
          escape_loop = true;
          break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          game_->React(event.key);
          break;
        case SDL_MOUSEMOTION:
          game_->React(event.motion, kWindowSize);
          break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          game_->React(event.button, kWindowSize);
          break;
        case SDL_MOUSEWHEEL:
          game_->React(event.wheel, kWindowSize);
          break;
      }
    }
    if (escape_loop || !game_->ongoing()) {
      break;
    }

    // Update the game
    if (!game_->Step(kGameLoopIntervalSec * tweaker_ctx.system_time_speed,
                     kWindowSize)) {
      mojgame::LOGGER().Error("Failed to step the game");
      loop_stat = -1;
      break;
    }

    // Render the objects
    mojgame::gl_rendering::clear_color_buffer();
    mojgame::gl_rendering::clear_depth_buffer();
    if (!game_->Render(kWindowSize)) {
      mojgame::LOGGER().Error("Failed to render the game");
      loop_stat = -1;
      break;
    }
    if (TwDraw() == 0) {
      mojgame::LOGGER().Error("Failed to render the tweak bar");
      loop_stat = -1;
      break;
    }
    SDL_GL_SwapWindow(window_);

    int finish_tick = SDL_GetTicks();
    int exec_tick = finish_tick - start_tick;
    Sleep(std::max<int>(kGameLoopInterval - exec_tick, 1));

    fps_counter_.Update(SDL_GetTicks());
    tweaker_ctx.system_actual_fps = fps_counter_.fps();
  }

  // Reset the minimum timer resolution
  timeEndPeriod(1);

  CleanUp();
  return loop_stat;
}

void GracefulRainApp::CleanUp() {
  mojgame::LOGGER().Info("Clean up the application");

  if (game_ != nullptr) {
    game_->Finalize();
    delete game_;
    game_ = nullptr;
  }
  if (tweak_bar_ != nullptr) {
    if (TwRemoveAllVars(tweak_bar_) == 0) {
      mojgame::LOGGER().Warn(
          "Failed to remove variables from tweak bar (errmsg: %s)",
          TwGetLastError());
    }
    if (TwDeleteBar(tweak_bar_) == 0) {
      mojgame::LOGGER().Warn("Failed to delete tweak bar (errmsg: %s)",
                             TwGetLastError());
    }
    tweak_bar_ = nullptr;
  }
  if (TwTerminate() == 0) {
    // Ignore the error to terminate the tweaker
    // because the tweaker library may be not initialized
  }
  if (context_ != nullptr) {
    SDL_GL_DeleteContext(context_);
  }
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
  }
  SDL_Quit();
}
