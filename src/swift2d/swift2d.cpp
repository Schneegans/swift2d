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

        // register all objects ------------------------------------------------
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
        // Application::destroy_instance();
        // MainLoop::destroy_instance();

        glfwTerminate();

        Logger::LOG_MESSAGE << "Bye!" << std::endl;
      }

      oalplus::Device audio_device_;
      oalplus::CurrentContext audio_context_;
  };

}

Swift2DContext* context;

void init(int argc, char** argv) {
  context = new Swift2DContext();
}

void clean_up() {
  delete context;
}

}
