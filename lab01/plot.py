import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

METHODS = {"RDTSC_ms": "RDTSC", "Monotonic_ms": "CLOCK_MONOTONIC"}
P = 0.95


def compute_stats(data: np.ndarray, p: float = P) -> dict:
    t_avg = data.mean()
    D = ((data - t_avg) ** 2).mean()
    sigma = np.sqrt(D)
    delta = 1.96 * sigma / np.sqrt(len(data))

    mask = np.abs(data - t_avg) <= 3 * sigma
    filtered = data[mask]
    t_avg_f = filtered.mean()
    D_f = ((filtered - t_avg_f) ** 2).mean()
    sigma_f = np.sqrt(D_f)
    delta_f = 1.96 * sigma_f / np.sqrt(len(filtered))

    sorted_d = np.sort(data)
    n = len(sorted_d)
    window = int(n * p)
    widths = sorted_d[window - 1:] - sorted_d[:n - window + 1]
    i = np.argmin(widths)

    return dict(
        t_min=data.min(),
        t_avg=t_avg, D=D, sigma=sigma, delta=delta,
        mask=mask, removed=(~mask).sum(),
        t_avg_f=t_avg_f, D_f=D_f, sigma_f=sigma_f, delta_f=delta_f,
        ci_low=sorted_d[i], ci_high=sorted_d[i + window - 1],
        t_avg_np=sorted_d[i:i + window].mean(),
    )


def save_latex(stats: dict, path: str):
    fmt = lambda x: f"{x:.3f}"
    ci = lambda lo, hi: f"[{fmt(lo)}; {fmt(hi)}]"

    rows = [
        ("$t_{\\min}$, мс",               lambda s: fmt(s["t_min"])),
        ("$t_{\\mathrm{avg}}(T)$, мс",    lambda s: fmt(s["t_avg"])),
        ("$D(T)$, мс$^2$",                lambda s: fmt(s["D"])),
        ("$\\sigma(T)$, мс",              lambda s: fmt(s["sigma"])),
        ("$\\Delta t$, мс",               lambda s: fmt(s["delta"])),
        ("$\\mathrm{ДИ}(T)$, мс",         lambda s: ci(s["t_avg"] - s["delta"], s["t_avg"] + s["delta"])),
        ("$t_{\\mathrm{avg}}(T')$, мс",   lambda s: fmt(s["t_avg_f"])),
        ("$D(T')$, мс$^2$",               lambda s: fmt(s["D_f"])),
        ("$\\sigma(T')$, мс",             lambda s: fmt(s["sigma_f"])),
        ("$\\Delta t'$, мс",              lambda s: fmt(s["delta_f"])),
        ("$\\mathrm{ДИ}(T')$, мс",        lambda s: ci(s["t_avg_f"] - s["delta_f"], s["t_avg_f"] + s["delta_f"])),
        ("$t_{\\mathrm{avg}}(T''')$, мс", lambda s: fmt(s["t_avg_np"])),
        ("$\\mathrm{ДИ}(T''')$, мс",      lambda s: ci(s["ci_low"], s["ci_high"])),
    ]

    cols = list(METHODS.values())
    lines = [
        "\\begin{tabular}{l" + "r" * len(cols) + "}",
        "\\toprule",
        "& " + " & ".join(cols) + " \\\\",
        "\\midrule",
        *[f"{name} & {' & '.join(fn(stats[col]) for col in METHODS)} \\\\" for name, fn in rows],
        "\\bottomrule",
        "\\end{tabular}",
    ]

    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")


def plot_hist(ax, data: np.ndarray, bins: np.ndarray, ylim: float):
    ax.hist(data, bins=bins, color="steelblue")
    ax.set_xlim(bins[0], bins[-1])
    ax.set_ylim(0, ylim)
    ax.set_xlabel("мс")
    ax.set_ylabel("кол-во")


def plot_ci(ax, s: dict):
    xs = [1, 2, 3, 4]
    ys = [s["t_min"], s["t_avg"], s["t_avg_f"], s["t_avg_np"]]
    yerr_lo = [0, s["delta"], s["delta_f"], s["t_avg_np"] - s["ci_low"]]
    yerr_hi = [0, s["delta"], s["delta_f"], s["ci_high"] - s["t_avg_np"]]
    ax.errorbar(xs, ys, yerr=[yerr_lo, yerr_hi],
                fmt="s", capsize=6, color="steelblue", markersize=7)
    ax.set_xticks(xs)
    ax.set_xticklabels(["$t_{min}$", "$t_{avg}(T)$", "$t_{avg}(T')$", "$t_{avg}(T''')$"], fontsize=8)
    ax.set_ylabel("мс")
    ax.grid(axis="y", linestyle=":", alpha=0.5)


def save_fig(fig, name: str):
    fig.savefig(f"output/{name}.png", dpi=150)
    plt.close(fig)


def main():
    df = pd.read_csv("output/results.csv")
    stats = {col: compute_stats(df[col].values) for col in METHODS}

    save_latex(stats, "output/stats.tex")

    x_max = max(df[col].max() for col in METHODS)
    bins = np.linspace(0, x_max, 101)

    y_raw = max(np.histogram(df[col].values, bins=bins)[0].max() for col in METHODS) * 1.05
    y_filtered = max(np.histogram(df[col].values[stats[col]["mask"]], bins=bins)[0].max() for col in METHODS) * 1.05

    for col, label in METHODS.items():
        s = stats[col]
        data = df[col].values

        fig, ax = plt.subplots(figsize=(7, 4))
        plot_hist(ax, data, bins, y_raw)
        save_fig(fig, f"{label}_raw")

        fig, ax = plt.subplots(figsize=(7, 4))
        plot_hist(ax, data[s["mask"]], bins, y_filtered)
        save_fig(fig, f"{label}_filtered")

        fig, ax = plt.subplots(figsize=(5, 5))
        plot_ci(ax, s)
        save_fig(fig, f"{label}_ci")


if __name__ == "__main__":
    main()
