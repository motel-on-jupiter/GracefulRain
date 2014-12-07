/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/GracefulRainApp.h"
#include "mojgame/catalogue/log_emitter/DebuggerConsoleLogEmitter.h"
#include "mojgame/catalogue/log_emitter/FileLogEmitter.h"
#include "mojgame/includer/crtdbg_include.h"
#include "mojgame/logging/Logger.h"

#define kDebugAlloc (-1)

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

#if defined(_MSC_VER) && defined(_DEBUG)
  // Validate the detection for memory leak
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetBreakAlloc(kDebugAlloc);
#endif /* defined(_MSC_VER) && defined(_DEBUG) */

  // Set up logger
  mojgame::LOGGER().PushEmitter(mojgame::DEBUGGER_CONSOLE_LOG_EMITTER());
  mojgame::LOGGER().PushEmitter(mojgame::STDERR_LOG_EMITTER());

  // Start the application
  GracefulRainApp app;
  return app.Run();
}
