#pragma once

namespace vextr::core {

/// @enum Align
/// @brief Specifies alignment within a container or space.
///
/// Used for both horizontal and vertical alignment of content within widgets
/// and layout slots.
enum class Align {
  Start,   ///< Align to the start (left for horizontal, top for vertical)
  Center,  ///< Align to the center
  End,     ///< Align to the end (right for horizontal, bottom for vertical)
  Stretch, ///< Stretch to fill the available space
};

} // namespace vextr::core