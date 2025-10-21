// --- include/core/RK4.hpp ---
#pragma once
#include "System.hpp"

namespace pc {

    struct RK4 {
        template <class Sys>
        void step(const Sys& sys, State& y, Real& t, Real dt) const {
            State k1{}, k2{}, k3{}, k4{}, yt{};

            // --- k1 ---
            sys.deriv(y, k1, t);

            // --- k2 ---
            yt = y;
            add_scaled(yt, k1, 0.5 * dt);
            sys.deriv(yt, k2, t + 0.5 * dt);

            // --- k3 ---
            yt = y;
            add_scaled(yt, k2, 0.5 * dt);
            sys.deriv(yt, k3, t + 0.5 * dt);

            // --- k4 ---
            yt = y;
            add_scaled(yt, k3, dt);
            sys.deriv(yt, k4, t + dt);

            // --- update ---
            y.theta  += (dt / 6.0) * (k1.theta  + 2*k2.theta  + 2*k3.theta  + k4.theta);
            y.omega  += (dt / 6.0) * (k1.omega  + 2*k2.omega  + 2*k3.omega  + k4.omega);
            y.theta2 += (dt / 6.0) * (k1.theta2 + 2*k2.theta2 + 2*k3.theta2 + k4.theta2);
            y.omega2 += (dt / 6.0) * (k1.omega2 + 2*k2.omega2 + 2*k3.omega2 + k4.omega2);

            t += dt;
        }

    private:
        static void add_scaled(State& y, const State& k, Real scale) {
            y.theta  += scale * k.theta;
            y.omega  += scale * k.omega;
            y.theta2 += scale * k.theta2;
            y.omega2 += scale * k.omega2;
        }
    };

}
