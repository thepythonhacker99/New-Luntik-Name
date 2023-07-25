#pragma once

#include "../Renderer/Window.h"

namespace Luntik {
class Game : Utils::NonCopyable {
public:
  Game();

  void run();

private:
  Renderer::Window *m_Window;
};
} // namespace Luntik
