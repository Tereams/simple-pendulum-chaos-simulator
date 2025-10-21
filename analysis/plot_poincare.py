#!/usr/bin/env python3
"""
Plot a Poincaré section by sampling when theta crosses a chosen phase with
a specified direction (e.g., theta = 0 with positive crossing).

Usage:
  python plot_poincare.py input.csv [output.png] [phase_rad] [direction]
  - phase_rad: the crossing phase in radians (default: 0.0)
  - direction: 'pos' for upward crossing, 'neg' for downward (default: 'pos')

For a single pendulum, a common choice is phase=0 and 'pos' crossings.
We record (theta_mod, omega) or simply (omega) at the crossing; here we scatter
(theta_mod, omega), with theta_mod wrapped to [-pi, pi] for completeness.
"""
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def wrap_angle(a):
    return (a + np.pi) % (2*np.pi) - np.pi

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    in_csv = sys.argv[1]
    out_png = sys.argv[2] if len(sys.argv) >= 3 else "poincare.png"
    phase = float(sys.argv[3]) if len(sys.argv) >= 4 else 0.0
    direction = sys.argv[4] if len(sys.argv) >= 5 else "pos"  # 'pos' or 'neg'

    df = pd.read_csv(in_csv)
    t = df["t"].values
    theta = wrap_angle(df["theta"].values)
    omega = df["omega"].values

    # Detect zero (phase) crossings: theta - phase changes sign
    phi = wrap_angle(theta - phase)
    s = np.sign(phi)
    s[s == 0] = 1  # treat exact zeros as positive side to avoid ambiguity

    poinc_t = []
    poinc_th = []
    poinc_om = []

    for i in range(1, len(t)):
        # crossing if sign flips
        if s[i] != s[i-1]:
            # Direction filter
            # 'pos': crossing with phi increasing through 0; approximate by omega > 0
            # 'neg': crossing with phi decreasing; approximate by omega < 0
            if direction == "pos" and omega[i] <= 0:
                continue
            if direction == "neg" and omega[i] >= 0:
                continue

            # Linear interpolation to estimate crossing time and state
            # phi(t) ~ phi0 + (dphi/dt)*dt; we just do linear interp in phi vs time
            t0, t1 = t[i-1], t[i]
            phi0, phi1 = phi[i-1], phi[i]
            w0, w1 = omega[i-1], omega[i]
            th0, th1 = theta[i-1], theta[i]
            # fraction where phi crosses 0
            if (phi1 - phi0) == 0:
                frac = 0.5
            else:
                frac = -phi0 / (phi1 - phi0)
                frac = np.clip(frac, 0.0, 1.0)

            tc = t0 + frac * (t1 - t0)
            thc = wrap_angle(th0 + frac * (th1 - th0))
            wc  = w0 + frac * (w1 - w0)

            poinc_t.append(tc)
            poinc_th.append(thc)
            poinc_om.append(wc)

    # Scatter the section
    plt.figure(figsize=(6, 6))
    plt.scatter(poinc_th, poinc_om, s=8, alpha=0.7)
    plt.xlabel("theta (rad)")
    plt.ylabel("omega (rad/s)")
    plt.title(f"Poincaré Section (phase={phase:.2f} rad, {direction} crossing)")
    plt.grid(True, ls=":")
    plt.tight_layout()
    plt.savefig(out_png, dpi=150)
    print(f"Crossings: {len(poinc_t)}")
    print(f"Saved: {out_png}")

if __name__ == "__main__":
    main()
