// --- include/core/Types.hpp ---
#pragma once
#include <cmath>

namespace pc {
    using Real = double;

    // State for single pendulum: angle (rad) and angular velocity (rad/s)
    struct State {
        Real theta{0.0};
        Real omega{0.0};
    };

    // Physical + damping parameters
    struct Params {
        Real g{9.81};   // gravity (m/s^2)
        Real L{1.0};    // length (m)
        Real b{0.0};    // linear damping on omega (1/s)
    };
}