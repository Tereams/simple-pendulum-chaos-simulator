// --- include/core/RK4.hpp ---
#pragma once
#include "System.hpp"

namespace pc {
    struct RK4 {
        template <class Sys>
        void step(const Sys& sys, State& y, Real& t, Real dt) const {
            State k1{}, k2{}, k3{}, k4{}, yt{};

            sys.deriv(y, k1, t);

            yt.theta = y.theta + 0.5 * dt * k1.theta;
            yt.omega = y.omega + 0.5 * dt * k1.omega;
            sys.deriv(yt, k2, t + 0.5 * dt);

            yt.theta = y.theta + 0.5 * dt * k2.theta;
            yt.omega = y.omega + 0.5 * dt * k2.omega;
            sys.deriv(yt, k3, t + 0.5 * dt);

            yt.theta = y.theta + dt * k3.theta;
            yt.omega = y.omega + dt * k3.omega;
            sys.deriv(yt, k4, t + dt);

            y.theta += (dt / 6.0) * (k1.theta + 2.0 * k2.theta + 2.0 * k3.theta + k4.theta);
            y.omega += (dt / 6.0) * (k1.omega + 2.0 * k2.omega + 2.0 * k3.omega + k4.omega);
            t += dt;
        }
    };
}
