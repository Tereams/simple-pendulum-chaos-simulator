// --- include/core/SinglePendulum.hpp ---
#pragma once
#include "System.hpp"

namespace pc {
    class SinglePendulum final : public System {
    public:
        explicit SinglePendulum(Params p) : p_(p) {}

        void deriv(const State& y, State& dydt, Real /*t*/) const override {
            // y.theta = angle, y.omega = angular velocity
            dydt.theta = y.omega;
            // equation: theta_ddot = -(g/L) * sin(theta) - b * omega
            dydt.omega = -(p_.g / p_.L) * std::sin(y.theta) - p_.b * y.omega;
        }

        Real energy(const State& y) const override {
            // Use unit mass m=1 for convenience
            const Real v = p_.L * y.omega; // tip linear speed
            const Real T = 0.5 * v * v;    // kinetic: 1/2 m v^2, m=1
            const Real U = (p_.g * p_.L) * (1.0 - std::cos(y.theta));
            return T + U;
        }

        const Params& params() const { return p_; }
    private:
        Params p_{};
    };
}