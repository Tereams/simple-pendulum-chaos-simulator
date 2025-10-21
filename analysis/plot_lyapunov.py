#!/usr/bin/env python3
"""
Estimate (finite-time) largest Lyapunov exponent using two trajectories.
You need two CSVs: a reference and a perturbed run with a tiny initial offset.

Usage:
  python plot_lyapunov.py ref.csv perturbed.csv [output.png]

Method:
  Compute separation d(t) = ||x2(t) - x1(t)|| in state space, here x=(theta,omega),
  with theta wrapped to [-pi, pi]. Estimate slope of ln d(t) over an early window.
"""
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def wrap_angle(a):
    return (a + np.pi) % (2*np.pi) - np.pi

def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return
    ref_csv = sys.argv[1]
    per_csv = sys.argv[2]
    out_png = sys.argv[3] if len(sys.argv) >= 4 else "lyapunov.png"

    df1 = pd.read_csv(ref_csv)
    df2 = pd.read_csv(per_csv)

    # Align by time; assume same dt and length; otherwise merge on nearest time
    # Simple assumption here for clarity:
    n = min(len(df1), len(df2))
    t = df1["t"].values[:n]
    th1 = wrap_angle(df1["theta"].values[:n])
    om1 = df1["omega"].values[:n]
    th2 = wrap_angle(df2["theta"].values[:n])
    om2 = df2["omega"].values[:n]

    # Distance in state space; for small angles this is reasonable.
    d = np.sqrt((wrap_angle(th2 - th1))**2 + (om2 - om1)**2)

    # Avoid zeros
    d = np.clip(d, 1e-16, None)
    lnd = np.log(d)

    # Choose an early-time window to fit a line (where linear divergence holds)
    # Heuristic: first 20% of the series
    k = max(10, int(0.2 * len(t)))
    T_fit = t[:k]
    Y_fit = lnd[:k]
    # Linear fit: lnd ~ a * t + b  =>  a ~ lambda (largest finite-time Lya)
    A = np.vstack([T_fit, np.ones_like(T_fit)]).T
    a, b = np.linalg.lstsq(A, Y_fit, rcond=None)[0]
    lambda_est = a

    # Plot ln d(t) and the fit
    plt.figure(figsize=(8, 5))
    plt.plot(t, lnd, lw=1, label="ln distance")
    plt.plot(T_fit, a*T_fit + b, lw=1.5, label=f"linear fit (λ ≈ {lambda_est:.4f})")
    plt.xlabel("time (s)")
    plt.ylabel("ln d(t)")
    plt.title("Finite-time Lyapunov Exponent Estimate")
    plt.grid(True, ls=":")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=150)
    print(f"Saved: {out_png}")
    print(f"Estimated largest Lyapunov exponent: {lambda_est:.6f}  (fit on first {k} samples)")

if __name__ == "__main__":
    main()
