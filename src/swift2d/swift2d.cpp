////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// header
#include <swift2d/swift2d.hpp>
#include <swift2d/utils/Logger.hpp>

#include <swift2d/openal.hpp>
#include <GLFW/glfw3.h>

namespace swift {

namespace {

  class Swift2DContext {

    public:
      Swift2DContext()
        : audio_device_()
        , audio_context_(audio_device_) {

        Logger::LOG_MESSAGE << "Booting Swift2D..." << std::endl;

        // init glfw -----------------------------------------------------------
        if (!glfwInit()) {
          Logger::LOG_ERROR << "Failed to initialize glfw3!" << std::endl;
        }

        glfwSetErrorCallback([](int error, const char* description){
          Logger::LOG_ERROR << "glfw3 error: " << description << std::endl;
        });

        Logger::LOG_MESSAGE << "Done!" << std::endl;
      }

      ~Swift2DContext() {
        Logger::LOG_MESSAGE << "Shutting down... " << std::endl;

        glfwTerminate();

        Logger::LOG_MESSAGE << "Bye!" << std::endl;
      }

      oalplus::Device audio_device_;
      oalplus::CurrentContext audio_context_;
  };

}

void init(int argc, char** argv) {
  static Swift2DContext context;
}

}
