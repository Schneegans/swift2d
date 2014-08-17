////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SWIFT2D_HPP
#define SWIFT2D_SWIFT2D_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/settings.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/stl_helpers.hpp>
#include <swift2d/math.hpp>
#include <swift2d/events.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/scene.hpp>
#include <swift2d/components.hpp>
#include <swift2d/behaviors.hpp>
#include <swift2d/shapes.hpp>
#include <swift2d/renderer.hpp>
#include <swift2d/databases.hpp>
#include <swift2d/materials.hpp>
#include <swift2d/particles.hpp>
#include <swift2d/trails.hpp>
#include <swift2d/physics.hpp>
#include <swift2d/input/keyboard_enums.hpp>
#include <swift2d/input/mouse_enums.hpp>
#include <swift2d/input/joystick_enums.hpp>
#include <swift2d/triggers/ShapeTrigger.hpp>
#include <swift2d/network.hpp>
#include <swift2d/audio.hpp>
#include <swift2d/gui.hpp>
#include <swift2d/steam/Steam.hpp>

#include <boost/asio.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Swift2D : public Singleton<Swift2D> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<> on_frame;

  // ---------------------------------------------------------------- properties
  Float      LoadingProgress;
  FPSCounter AppFPS;
  FPSCounter RenderFPS;

  // ------------------------------------------------------------ public methods
  void init(int argc, char** argv);

  void start();
  void stop();

  void clean_up();

  void display(SceneObjectPtr const& scene, CameraComponentPtr const& camera);

  std::string get_tmp_file(std::string const& suffix = "tmp") const;
  std::string get_resource(std::string const& type, std::string const& file) const;
  std::string make_absolute(std::string const& file) const;

  friend class Singleton<Swift2D>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Swift2D();
  ~Swift2D() {}

  Pipeline pipeline_;
  Renderer renderer_;
  WindowPtr window_;

  std::string executable_;
  boost::asio::signal_set signals_;

  oalplus::Device audio_device_;
  oalplus::CurrentContext audio_context_;
};

}

#endif  // SWIFT2D_SWIFT2D_HPP
