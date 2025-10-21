#!/usr/bin/env python3
"""
Plot energy drift E(t) - E(0) to assess numerical stability.
Usage:
  python plot_energy_drift.py input.csv [output.png]
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
    out_png = sys.argv[2] if len(sys.argv) >= 3 else "energy_drift.png"

    df = pd.read_csv(in_csv)
    t = df["t"].values
    E = df["energy"].values
    dE = E - E[0]

    plt.figure(figsize=(8, 4))
    plt.plot(t, dE, lw=1)
    plt.xlabel("time (s)")
    plt.ylabel("E(t) - E(0)")
    plt.title("Energy Drift")
    plt.grid(True, ls=":")
    plt.tight_layout()
    plt.savefig(out_png, dpi=150)
    print(f"Saved: {out_png}")

if __name__ == "__main__":
    main()
