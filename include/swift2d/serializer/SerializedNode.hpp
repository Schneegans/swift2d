////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SERIALIZED_NODE_HPP
#define SWIFT2D_SERIALIZED_NODE_HPP

// swift2d headers
#include <swift2d/math/math.hpp>

namespace swift {

/**
 * Stores information on a light for rendering.
 *
 * This is a struct used for serializing the graph.
 *
 * essentially the same as a std::pair<math::mat4, configuration_type>
 */
template <typename configuration_type> struct SerializedNode {

  SerializedNode() : transform(glm::mat4(1.0f)), data() {}

  /**
   * Constructor.
   *
   * This creates a new serialized node.
   *
   * \param transform        The global transformation of this node.
   * \param color            The color of the light.
   */
  SerializedNode(glm::mat4 const& t, configuration_type const& d)
      : transform(t), data(d) {}

  /**
   * The global transformation of this node.
   */
  glm::mat4 transform;
  configuration_type data;
};

template <typename T>
inline SerializedNode<T> make_serialized_node(glm::mat4 const& t, T const& d)
{
  return SerializedNode<T>(t, d);
}

}

#endif  // SWIFT2D_SERIALIZED_NODE_HPP
