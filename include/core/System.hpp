// --- include/core/System.hpp ---
#pragma once
#include "Types.hpp"

namespace pc {
    struct System {
        virtual ~System() = default;
        // dy/dt = f(y, t)
        virtual void deriv(const State& y, State& dydt, Real t) const = 0;
        // mechanical energy (for diagnostics)
        virtual Real energy(const State& y) const = 0;
    };
}