#!/usr/bin/env python3
"""
Plot time-series of angle (theta) and angular velocity (omega).
Usage:
  python plot_timeseries.py input.csv [output.png]
"""
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    in_csv = sys.argv[1]
    out_png = sys.argv[2] if len(sys.argv) >= 3 else "timeseries.png"

    df = pd.read_csv(in_csv)
    t = df["t"].values
    theta = df["theta"].values
    omega = df["omega"].values

    # Optional: wrap theta to [-pi, pi] for nicer viewing
    theta = (theta + np.pi) % (2*np.pi) - np.pi

    # Two aligned subplots (simple, readable)
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), sharex=True)
    ax1.plot(t, theta, lw=1)
    ax1.set_ylabel("theta (rad)")
    ax1.grid(True, ls=":")

    ax2.plot(t, omega, lw=1)
    ax2.set_xlabel("time (s)")
    ax2.set_ylabel("omega (rad/s)")
    ax2.grid(True, ls=":")

    fig.suptitle("Pendulum Time Series")
    fig.tight_layout()
    fig.savefig(out_png, dpi=150)
    print(f"Saved: {out_png}")

if __name__ == "__main__":
    main()
