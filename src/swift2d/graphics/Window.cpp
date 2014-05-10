////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Window::open() {



  if (!window_) {
    window_ = std::shared_ptr<sf::Window>(
      new sf::Window(sf::VideoMode(800, 600), "FibreGlass3D", sf::Style::Default)
    );

    glewInit();
    // set_active(true);

    // window_->setVerticalSyncEnabled(true);

    // render_context_.gl = new oglplus::Context();

    on_resize(800, 600);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::process_input() {

  if (window_) {
    sf::Event event;
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        std::cout << "close" << std::endl;
      } else if (event.type == sf::Event::Resized) {
        std::cout << "resize" << std::endl;
        // on_resize(event.size.width, event.size.height);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::set_active(bool active) {
  if (window_) {
    window_->setActive(active);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::display() {
  if (window_) {
    window_->display();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::on_resize(int width, int height) {
  if (width != render_context_.width || height != render_context_.height) {

    // set_active(true);

    render_context_.width = width;
    render_context_.height = height;

    render_context_.gl.Viewport(width, height);
  }

}

////////////////////////////////////////////////////////////////////////////////

}
