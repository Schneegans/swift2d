////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SERIALIZER_HPP
#define SWIFT2D_SERIALIZER_HPP


namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Serializer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Serializer(Scene const& scene);

  void serialize(std::vector<Frustum> const& frusta);

  std::vector<std::shared_ptr<SerializedScene>> const& get_scenes() const { return scenes_; }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::vector<std::thread*> workers_;
  std::vector<std::shared_ptr<SerializedScene>> scenes_;
};

}

#endif  // SWIFT2D_SERIALIZER_HPP
