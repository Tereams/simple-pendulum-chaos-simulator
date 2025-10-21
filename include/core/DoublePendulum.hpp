// --- include/core/DoublePendulum.hpp ---
#pragma once
#include "System.hpp"
#include <cmath>

namespace pc {

    class DoublePendulum final : public System {
    public:
        explicit DoublePendulum(Params p) : p_(p) {}

        // State: y.theta1, y.omega1, y.theta2, y.omega2
        // Output: dydt.theta1, dydt.omega1, dydt.theta2, dydt.omega2
        void deriv(const State& y, State& dydt, Real /*t*/) const override {
            const Real g = p_.g;
            const Real L1 = p_.L;
            const Real L2 = p_.L2;   // expand the Params
            const Real m1 = p_.m1;
            const Real m2 = p_.m2;
            const Real b  = p_.b;

            const Real th1 = y.theta;
            const Real th2 = y.theta2;
            const Real w1  = y.omega;
            const Real w2  = y.omega2;

            const Real delta = th2 - th1;
            const Real sin_d = std::sin(delta);
            const Real cos_d = std::cos(delta);

            const Real denom = (m1 + m2) * L1 - m2 * L1 * cos_d * cos_d;

            // ¦È1''
            const Real num1 = m2 * L1 * w1 * w1 * sin_d * cos_d
                            + m2 * g * std::sin(th2) * cos_d
                            + m2 * L2 * w2 * w2 * sin_d
                            - (m1 + m2) * g * std::sin(th1);
            const Real th1_ddot = num1 / denom - b * w1;

            // ¦È2''
            const Real num2 = -L2 / L1 * w2 * w2 * sin_d * cos_d
                            + (m1 + m2) * g * std::sin(th1) * cos_d
                            - (m1 + m2) * L1 * w1 * w1 * sin_d
                            - (m1 + m2) * g * std::sin(th2);
            const Real th2_ddot = num2 / ((L2 / L1) * denom) - b * w2;

            dydt.theta = w1;
            dydt.omega = th1_ddot;
            dydt.theta2 = w2;
            dydt.omega2 = th2_ddot;
        }

        Real energy(const State& y) const override {
            const Real g = p_.g;
            const Real L1 = p_.L;
            const Real L2 = p_.L2;
            const Real m1 = p_.m1;
            const Real m2 = p_.m2;

            const Real th1 = y.theta;
            const Real th2 = y.theta2;
            const Real w1  = y.omega;
            const Real w2  = y.omega2;

            const Real y1 = -L1 * std::cos(th1);
            const Real y2 = y1 - L2 * std::cos(th2);

            const Real v1_sq = (L1 * w1) * (L1 * w1);
            const Real v2_sq = v1_sq + (L2 * w2) * (L2 * w2)
                             + 2 * L1 * L2 * w1 * w2 * std::cos(th1 - th2);

            const Real T = 0.5 * (m1 * v1_sq + m2 * v2_sq);
            const Real U = (m1 + m2) * g * L1 * (1 - std::cos(th1))
                         + m2 * g * L2 * (1 - std::cos(th2));

            return T + U;
        }

        const Params& params() const { return p_; }

    private:
        Params p_{}; // Params: g, L, L2, m1, m2, b
    };

}
