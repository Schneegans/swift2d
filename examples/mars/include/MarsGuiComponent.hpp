////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_GUI_COMPONENT_HPP
#define MARS_GUI_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

// shared pointer type definition ----------------------------------------------
class MarsGuiComponent;
typedef std::shared_ptr<MarsGuiComponent>       MarsGuiComponentPtr;
typedef std::shared_ptr<const MarsGuiComponent> ConstMarsGuiComponentPtr;

class MarsGuiComponent : public swift::GuiComponent {
 public:
  MarsGuiComponent(
    std::unordered_map<std::string, std::function<void()>> const& callbacks,
    bool hidable);

  void show();
  void hide();

  bool is_visible() const;

  virtual void update(double time);

 private:
  swift::AnimatedFloat top_;
  bool hidable_;
  bool visible_;

};

// -----------------------------------------------------------------------------

#endif // MARS_GUI_COMPONENT_HPP
