////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SHADER_INCLUDES_HPP
#define SWIFT2D_SHADER_INCLUDES_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ShaderIncludes : public Singleton<ShaderIncludes> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void process(std::string& input) const;

  void add_include(std::string const& identifier, std::string const& code);

  friend class Singleton<ShaderIncludes>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  ShaderIncludes();
  ~ShaderIncludes() {};

  bool process_impl(std::string& input) const;

  std::unordered_map<std::string, std::string> snippets_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SHADER_INCLUDES_HPP
