#!/usr/bin/env python3
"""
Plot phase portrait (theta vs. omega).
Usage:
  python plot_phase.py input.csv [output.png]
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
    out_png = sys.argv[2] if len(sys.argv) >= 3 else "phase.png"

    df = pd.read_csv(in_csv)
    theta = df["theta"].values
    omega = df["omega"].values

    # Wrap theta to [-pi, pi] for clearer loops
    theta = (theta + np.pi) % (2*np.pi) - np.pi

    plt.figure(figsize=(6, 6))
    plt.plot(theta, omega, lw=0.8)
    plt.xlabel("theta (rad)")
    plt.ylabel("omega (rad/s)")
    plt.title("Phase Portrait")
    plt.grid(True, ls=":")
    plt.tight_layout()
    plt.savefig(out_png, dpi=150)
    print(f"Saved: {out_png}")

if __name__ == "__main__":
    main()
