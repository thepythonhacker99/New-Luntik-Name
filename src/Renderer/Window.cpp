#include "Window.h"

#include "SFML/System/Vector2.hpp"

#include <cstdint>
#include <string>

namespace Luntik::Renderer {
Window::Window(const std::string &windowName, uint32_t height)
    : m_Camera(height), m_WindowName(windowName), m_Height(height) {
  m_Window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(1280, 720)),
                                  m_WindowName, sf::Style::Default);

  m_Window->setView(m_Camera);

  m_FullScreenVideoMode = sf::VideoMode::getFullscreenModes()[0];
}

Window::~Window() { delete m_Window; }

void Window::initWindow() {
  if (!m_FullScreen) {
    m_Window->create(sf::VideoMode(sf::Vector2u(1280, 720)), m_WindowName,
                     sf::Style::Default);
  } else {
    m_Window->create(m_FullScreenVideoMode, m_WindowName,
                     sf::Style::Fullscreen);
  }
}

void Window::render(float deltaTime) {
  m_Camera.tick(deltaTime, m_Window->getSize());
  m_Window->setView(m_Camera);

  m_Window->clear(sf::Color(15, 15, 30));

  sf::Event e;

  while (m_Window->pollEvent(e)) {
    switch (e.type) {
    case sf::Event::Closed:
      m_Window->close();
      break;
    default:
      break;
    }
  }
}

void Window::display() { m_Window->display(); }

bool Window::isOpen() const { return m_Window->isOpen(); }

void Window::toggleFullScreen() {
  m_FullScreen = !m_FullScreen;
  initWindow();
}

void Window::setFullScreen(bool fullScreen) {
  if (fullScreen == m_FullScreen)
    return;

  toggleFullScreen();
}

bool Window::getFullScreen() const { return m_FullScreen; }

Camera *Window::getCamera() { return &m_Camera; }

sf::RenderWindow *Window::getInternalWindow() const { return m_Window; }
} // namespace Luntik::Renderer
