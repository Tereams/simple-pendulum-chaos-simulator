// --- include/core/Types.hpp ---
#pragma once
#include <cmath>

namespace pc {
    using Real = double;

    // ---- State Definition ----
    // Can represent both single and double pendulum.
    // Unused variables should remain at their default value (0).
    struct State {
        // Used by single pendulum
        Real theta{0.0};
        Real omega{0.0};

        // Additional variables for double pendulum
        Real theta2{0.0};
        Real omega2{0.0};
    };

    // ---- Parameter Definition ----
    struct Params {
        // Common parameters
        Real g{9.81};   // Gravitational acceleration
        Real b{0.0};    // Damping coefficient

        // L used for single pendulum; L1/L2 for double pendulum
        Real L{1.0};    // Length of the first pendulum
        Real L2{1.0};   // Length of the second pendulum

        // Mass parameters for double pendulum
        Real m1{1.0};
        Real m2{1.0};
    };
}
