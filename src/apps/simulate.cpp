// --- src/apps/simulate.cpp ---
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "core/SinglePendulum.hpp"
#include "core/RK4.hpp"

int main() {
    using namespace pc;

    // --- minimal hardcoded run ---
    Params p;           // g=9.81, L=1.0, b=0.0
    p.b = 0.0;          // set small damping like 0.01 if you want

    SinglePendulum sys{p};
    RK4 integrator{};

    // initial state: 45 degrees, zero initial angular velocity
    State y{ M_PI / 4.0, 0.0 };
    Real t = 0.0;
    const Real dt = 0.001;   // 1 ms
    const Real T  = 10.0;    // 10 s

    // CSV header to stdout (redirect to a file if you want)
    std::cout << "t,theta,omega,energy\n";

    for (; t <= T + 1e-12; ) {
        const Real E = sys.energy(y);
        std::cout << t << "," << y.theta << "," << y.omega << "," << E << "\n";
        integrator.step(sys, y, t, dt);
    }

    return 0;
}