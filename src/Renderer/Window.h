#pragma once

#include "Camera.h"

#include "../Utils/NonCopyable.h"
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

#include <cstdint>
#include <memory>

namespace Luntik::Renderer {
class Window : Utils::NonCopyable {
public:
  Window(const std::string &windowName, uint32_t height);
  ~Window();

  void render(float deltaTime);
  void display();

  sf::RenderWindow *getInternalWindow() const;

  bool isOpen() const;

  Camera *getCamera();

  bool getFullScreen() const;

  void setFullScreen(bool fullScreen);

  void toggleFullScreen();

private:
  void initWindow();

  bool m_FullScreen = false;

  sf::RenderWindow *m_Window;
  Camera m_Camera;

  std::string m_WindowName;

  uint32_t m_Height;

  sf::VideoMode m_FullScreenVideoMode;
};
} // namespace Luntik::Renderer
