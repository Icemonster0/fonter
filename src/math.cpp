#include "math.hpp"

namespace math {

float quad_interp(float t) {
    return (t < 0.5f) ?
           (2.f * t*t) :
           (-2.f * (t-1.f)*(t-1.f) + 1.f);
}

float cube_interp(float t) {
    float t_minus_one = (t-1.f);
    return (t < 0.5f) ?
           (4.f * t*t*t) :
           (4.f * t_minus_one*t_minus_one*t_minus_one + 1.f);
}

} /* math */
