////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/Swift2D.hpp>

#include <swift2d/utils/Logger.hpp>
#include <swift2d/openal.hpp>

// singletons which are not included by default
#include <swift2d/materials/DirectionalLightShader.hpp>
#include <swift2d/materials/PointLightShader.hpp>
#include <swift2d/materials/ShaderIncludes.hpp>
#include <swift2d/materials/MaterialShaderFactory.hpp>
#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/particles/HeatParticleShader.hpp>
#include <swift2d/particles/PointParticleShader.hpp>
#include <swift2d/particles/LightParticleShader.hpp>
#include <swift2d/particles/ParticleUpdateShader.hpp>
#include <swift2d/particles/SpriteParticleShader.hpp>
#include <swift2d/trails/TrailUpdateShader.hpp>
#include <swift2d/trails/TrailShader.hpp>
#include <swift2d/textures/NoiseTexture.hpp>
#include <swift2d/textures/DefaultTexture.hpp>
#include <swift2d/textures/DefaultAnimatedTexture.hpp>

#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Swift2D::Swift2D()
  : LoadingProgress(0.f)
  , AppFPS(20)
  , RenderFPS(20)
  , pipeline_()
  , renderer_(pipeline_)
  , window_(WindowManager::instance()->get_default())
  , executable_()
  , signals_(MainLoop::instance()->get_io_service(), SIGINT, SIGTERM)
  , audio_device_()
  , audio_context_(audio_device_) {

  pipeline_.set_output_window(window_);
}

////////////////////////////////////////////////////////////////////////////////

void Swift2D::init(int argc, char** argv) {

  Logger::LOG_MESSAGE << "Booting Swift2D..." << std::endl;

  executable_ = boost::filesystem::system_complete(argv[0]).normalize().remove_filename().string();

  // register all objects ------------------------------------------------------
  Object::init<AnimatedSpriteComponent>();
  Object::init<AnimatedTexture>();
  Object::init<BoxCollisionShape>();
  Object::init<CameraComponent>();
  Object::init<CircleCollisionShape>();
  Object::init<CircularShape>();
  Object::init<DeleteOnLeaveBehavior>();
  Object::init<DirectionalLightComponent>();
  Object::init<DynamicBodyComponent>();
  Object::init<GravitySourceComponent>();
  Object::init<GuiComponent>();
  Object::init<GuiComponent>();
  Object::init<HeatParticleSystemComponent>();
  Object::init<LightParticleSystemComponent>();
  Object::init<ListenerComponent>();
  Object::init<Material>();
  Object::init<MoveBehavior>();
  Object::init<OffsetBehavior>();
  Object::init<ParticleEmitterComponent>();
  Object::init<PointLightComponent>();
  Object::init<PointParticleSystemComponent>();
  Object::init<PolygonCollisionShape>();
  Object::init<SceneObject>();
  Object::init<Sound>();
  Object::init<SoundComponent>();
  Object::init<SpriteComponent>();
  Object::init<SpriteParticleSystemComponent>();
  Object::init<StaticBodyComponent>();
  Object::init<Texture>();
  Object::init<TimedDeleteBehavior>();
  Object::init<TrailEmitterComponent>();
  Object::init<TrailSystemComponent>();

  // init glfw -----------------------------------------------------------------
  if (!glfwInit()) {
    Logger::LOG_ERROR << "Failed to initialize glfw3!" << std::endl;
  }

  glfwSetErrorCallback([](int error, const char* description){
    Logger::LOG_ERROR << "glfw3 error: " << description << std::endl;
  });

  signals_.async_wait([this](boost::system::error_code const& error,
                          int signal_number){
    if (!error) {
      stop();
    }
  });

  Logger::LOG_MESSAGE << "Done!" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void Swift2D::start() {
  AppFPS.start();
  RenderFPS.start();

  MainLoop::instance()->start();
}

////////////////////////////////////////////////////////////////////////////////

void Swift2D::stop() {
  renderer_.stop();
  MainLoop::instance()->stop();
}

////////////////////////////////////////////////////////////////////////////////

void Swift2D::display(SceneObjectPtr const& scene, CameraComponentPtr const& camera) {
  renderer_.process(scene, camera);
}

////////////////////////////////////////////////////////////////////////////////

void Swift2D::clean_up() {
  Logger::LOG_MESSAGE << "Shutting down... " << std::endl;

  DefaultAnimatedTexture::destroy_instance();
  DefaultTexture::destroy_instance();
  DirectionalLightShader::destroy_instance();
  GuiShader::destroy_instance();
  HeatParticleShader::destroy_instance();
  LightParticleShader::destroy_instance();
  MaterialDatabase::destroy_instance();
  MaterialShaderFactory::destroy_instance();
  NoiseTexture::destroy_instance();
  ParticleUpdateShader::destroy_instance();
  PointLightShader::destroy_instance();
  PointParticleShader::destroy_instance();
  Quad::destroy_instance();
  ShaderIncludes::destroy_instance();
  SoundDatabase::destroy_instance();
  SpriteParticleShader::destroy_instance();
  TextureDatabase::destroy_instance();
  TrailShader::destroy_instance();
  TrailUpdateShader::destroy_instance();
  Steam::destroy_instance();

  // SceneManager::destroy_instance();
  // Physics::destroy_instance();

  // Interface::destroy_instance();
  // WindowManager::destroy_instance();
  // MainLoop::destroy_instance();

  glfwTerminate();

  Logger::LOG_MESSAGE << "Bye!" << std::endl;

  delete this;
}

////////////////////////////////////////////////////////////////////////////////

std::string Swift2D::get_resource(std::string const& type, std::string const& file) const {
  return executable_ + "/resources/" + type + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

std::string Swift2D::make_absolute(std::string const& file) const {
  return executable_ + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

}
