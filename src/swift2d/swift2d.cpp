////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/swift2d.hpp>

#include <swift2d/utils/Logger.hpp>
#include <swift2d/openal.hpp>

// singletons which are not included by default
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
#include <swift2d/trails/TexturedTrailShader.hpp>
#include <swift2d/trails/ColoredTrailShader.hpp>
#include <swift2d/textures/NoiseTexture.hpp>
#include <swift2d/textures/DefaultTexture.hpp>
#include <swift2d/textures/DefaultTexture3D.hpp>
#include <swift2d/properties/AnimatedProperty.hpp>

#include <GLFW/glfw3.h>

namespace swift {

namespace {

  class Swift2DContext {

   public:
    Swift2DContext()
      : audio_device_()
      , audio_context_(audio_device_) {

      LOG_MESSAGE << "Booting Swift2D..." << std::endl;

      // register all objects --------------------------------------------------
      Object::init<AnimatedDouble>();
      Object::init<AnimatedFloat>();
      Object::init<AnimatedGuiSpriteComponent>();
      Object::init<AnimatedSpriteComponent>();
      Object::init<AudioComponent>();
      Object::init<BoxCollisionShape>();
      Object::init<CameraComponent>();
      Object::init<CircleCollisionShape>();
      Object::init<CircularShape>();
      Object::init<DeleteOnLeaveBehavior>();
      Object::init<DirectionalLightComponent>();
      Object::init<DynamicBodyComponent>();
      Object::init<EngineSettings>();
      Object::init<FullscreenGuiSpriteComponent>();
      Object::init<FullscreenSpriteComponent>();
      Object::init<GravitySourceComponent>();
      Object::init<GuiComponent>();
      Object::init<GuiSpriteComponent>();
      Object::init<HeatParticleSystemComponent>();
      Object::init<HeatSpriteComponent>();
      Object::init<LifeComponent>();
      Object::init<LightParticleSystemComponent>();
      Object::init<ListenerComponent>();
      Object::init<Material>();
      Object::init<MoveBehavior>();
      Object::init<Music>();
      Object::init<NetworkPositionUpdateComponent>();
      Object::init<OffsetBehavior>();
      Object::init<ParticleEmitterComponent>();
      Object::init<PointLightComponent>();
      Object::init<PointParticleSystemComponent>();
      Object::init<PolygonCollisionShape>();
      Object::init<RespawnComponent>();
      Object::init<SceneObject>();
      Object::init<ScoreComponent>();
      Object::init<Sound>();
      Object::init<SpriteComponent>();
      Object::init<SpriteParticleSystemComponent>();
      Object::init<StaticBodyComponent>();
      Object::init<Texture3D>();
      Object::init<Texture::Layer>();
      Object::init<Texture>();
      Object::init<TimedDeleteBehavior>();
      Object::init<TrailEmitterComponent>();
      Object::init<TrailSystemComponent>();

      // init glfw -------------------------------------------------------------
      if (!glfwInit()) {
        LOG_ERROR << "Failed to initialize glfw3!" << std::endl;
      }

      glfwSetErrorCallback([](int error, const char* description){
        LOG_ERROR << "glfw3 error: " << description << std::endl;
      });

      LOG_MESSAGE << "Done!" << std::endl;
    }

    ~Swift2DContext() {

      LOG_MESSAGE << "Shutting down... " << std::endl;

      DefaultTexture3D::destroy_instance();
      DefaultTexture::destroy_instance();
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
      TexturedTrailShader::destroy_instance();
      ColoredTrailShader::destroy_instance();
      TrailUpdateShader::destroy_instance();
      SettingsWrapper::destroy_instance();
      Application::destroy_instance();
      Paths::destroy_instance();
      SceneManager::destroy_instance();

      // Physics::destroy_instance();
      // Interface::destroy_instance();
      // MainLoop::destroy_instance();
      // WindowManager::destroy_instance();

      glfwTerminate();

      LOG_MESSAGE << "Bye!" << std::endl;

      // Leave at this position! SteamAPI_Shutdown() seems to kill the process
      // under some conditions.
      Steam::destroy_instance();
    }

    oalplus::Device audio_device_;
    oalplus::CurrentContext audio_context_;
  };

  Swift2DContext* ctx = nullptr;

}

////////////////////////////////////////////////////////////////////////////////

void init() {
  clean_up();
  ctx = new Swift2DContext();
}

////////////////////////////////////////////////////////////////////////////////

void clean_up() {
  if (ctx) {
    delete ctx;
    ctx = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
