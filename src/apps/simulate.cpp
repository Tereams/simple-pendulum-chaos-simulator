// --- src/apps/simulate.cpp ---
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "core/SinglePendulum.hpp"
#include "core/RK4.hpp"

int main() {
    using namespace pc;

    // Parse command-line argument: output file path (optional)
    std::string out_path = (argc >= 2) ? std::string(argv[1]) : "output/single_pendulum.csv";

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
    ofs << "t,theta,omega,energy\n";

    for (; t <= T + 1e-12; ) {
        const Real E = sys.energy(y);
        ofs << t << "," << y.theta << "," << y.omega << "," << E << "\n";
        integrator.step(sys, y, t, dt);
    }

    ofs.close();
    std::cout << "Wrote CSV to: " << out_path << "\n";
    return 0;
}