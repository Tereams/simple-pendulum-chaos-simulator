// --- src/apps/simulate_double.cpp ---
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cmath>
#include "core/DoublePendulum.hpp"
#include "core/RK4.hpp"

int main(int argc, char** argv) {
    using namespace pc;

    // Parse command-line argument: output file path (optional)
    std::string out_path = (argc >= 2) ? std::string(argv[1]) : "output/double_pendulum.csv";

    // Ensure the output directory exists (if the path contains directories)
    try {
        std::filesystem::path p(out_path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            std::filesystem::create_directories(p.parent_path());
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to create output directory: " << e.what() << "\n";
        return 1;
    }

    // Open the output CSV file
    std::ofstream ofs(out_path, std::ios::out | std::ios::trunc);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << out_path << "\n";
        return 1;
    }

    // --- parameters ---
    Params p;
    p.L  = 1.0;    // first rod length
    p.L2 = 1.0;    // second rod length
    p.m1 = 1.0;    // first mass
    p.m2 = 1.0;    // second mass
    p.g  = 9.81;   // gravity
    p.b  = 0.01;   // small damping

    DoublePendulum sys{p};
    RK4 integrator{};

    // initial state: both pendulums start displaced
    State y{};
    y.theta  = M_PI / 2.0;   // upper pendulum
    y.theta2 = M_PI / 4.0;   // lower pendulum
    y.omega  = 0.0;
    y.omega2 = 0.0;

    Real t = 0.0;
    const Real dt = 0.001;   // integration step
    const Real T  = 20.0;    // total duration

    ofs << "t,theta1,omega1,theta2,omega2,energy\n";

    for (; t <= T + 1e-12; ) {
        const Real E = sys.energy(y);
        ofs << t << "," << y.theta << "," << y.omega << ","
            << y.theta2 << "," << y.omega2 << "," << E << "\n";
        integrator.step(sys, y, t, dt);
    }

    ofs.close();
    std::cout << "Wrote CSV to: " << out_path << "\n";
    return 0;
}
