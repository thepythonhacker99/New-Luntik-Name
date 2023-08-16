#include "Math.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Utils {
float lerp(float x, float y, float t) { return x + t * (y - x); }
} // namespace Luntik::Utils
