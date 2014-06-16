////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_HPP
#define PLAYER_HPP

// includes  -------------------------------------------------------------------
#include "Mover.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Player : public NetworkObject<Player> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static void init() {
    NetworkObject<Player>::init("PlayerType");
  }

  Player(bool is_local = false)
    : update_ticker_(Ticker::create(5.0)) {

    auto scene = SceneManager::instance()->get_default();
    player_ = scene->add_object();
    player_->Transform = math::make_scale(0.1);

    MoveBehaviorPtr mover;
    if (is_local) mover = std::make_shared<Mover>();
    else          mover = MoveBehavior::create();
    player_->add(mover);

    auto listener = player_->add<ListenerComponent>();
    listener->Volume = 1.0;

    auto ship = player_->add<SpriteComponent>();
    ship->Depth = 1.0f;
    ship->Material = MaterialDatabase::instance()->get("ship");

    auto light = player_->add<LightComponent>();
    light->Depth = 1.0f;
    light->Transform = math::make_scale(20);
    light->Material = MaterialDatabase::instance()->get("light");

    if (is_local) {
      update_ticker_->start();
      update_ticker_->on_tick.connect([&](){
        position_update_.set(math::get_translate(player_->Transform.get()));
        rotation_update_.set(math::get_rotation(player_->Transform.get()));
      });
    } else {
      position_update_.on_change().connect([&](math::vec2 const& val) {
        auto transform(player_->Transform.get());
        math::set_translate(transform, val);
        player_->Transform.set(transform);
      });
      rotation_update_.on_change().connect([&](float val) {
        auto transform(player_->Transform.get());
        math::set_rotation(transform, val);
        player_->Transform.set(transform);
      });
    }

    distribute_member(&mover->LinearSpeed);
    distribute_member(&mover->AngularSpeed);
    distribute_member(&position_update_);
    distribute_member(&rotation_update_);
  }

 ~Player() {
    update_ticker_->stop();
    auto scene = SceneManager::instance()->get_default();
    scene->remove(player_);
 }

 private:
  SceneObjectPtr player_;
  Vec2           position_update_;
  Float          rotation_update_;
  TickerPtr      update_ticker_;
};

#endif  // PLAYER_HPP
