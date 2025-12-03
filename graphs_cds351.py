import matplotlib.pyplot as plt

# -----------------------------------
# Raw data (N = 100,000,000)
# -----------------------------------

# Serial baseline time (MODE=0, THREADS=1, N=1e8)
T_serial = 0.561683

# Thread counts
threads = [1, 2, 4, 8, 16]

# Threaded (MODE=1) average times
T_threaded = [
    0.680531,   # 1 thread
    0.341910,   # 2 threads
    0.177527,   # 4 threads
    0.097633,   # 8 threads
    0.0673014   # 16 threads
]

# OpenMP (MODE=2) average times
T_openmp = [
    0.557188,   # 1 thread
    0.340810,   # 2 threads
    0.175302,   # 4 threads
    0.0929905,  # 8 threads
    0.0574771   # 16 threads
]

# -----------------------------------
# Compute speedup and efficiency
# -----------------------------------

S_threaded = []   # speedup for std::thread
S_openmp = []     # speedup for OpenMP
E_threaded = []   # efficiency for std::thread
E_openmp = []     # efficiency for OpenMP

for i in range(len(threads)):
    p = threads[i]

    # Threaded speedup and efficiency
    s_thr = T_serial / T_threaded[i]
    e_thr = s_thr / p
    S_threaded.append(s_thr)
    E_threaded.append(e_thr)

    # OpenMP speedup and efficiency
    s_omp = T_serial / T_openmp[i]
    e_omp = s_omp / p
    S_openmp.append(s_omp)
    E_openmp.append(e_omp)

# -----------------------------------
# Print tables
# -----------------------------------

print("=== Threaded (std::thread) Results ===")
print("Threads |   Time (s) |  Speedup | Efficiency")
print("---------------------------------------------")
for i in range(len(threads)):
    print(f"{threads[i]:7d} | {T_threaded[i]:10.6f} | {S_threaded[i]:8.3f} | {E_threaded[i]:10.3f}")
print()

print("=== OpenMP Results ===")
print("Threads |   Time (s) |  Speedup | Efficiency")
print("---------------------------------------------")
for i in range(len(threads)):
    print(f"{threads[i]:7d} | {T_openmp[i]:10.6f} | {S_openmp[i]:8.3f} | {E_openmp[i]:10.3f}")
print()

# -----------------------------------
# Plot: Runtime vs Threads
# -----------------------------------

plt.figure()
plt.plot(threads, T_threaded, marker='o', label='std::thread')
plt.plot(threads, T_openmp, marker='o', label='OpenMP')
plt.xlabel("Number of Threads")
plt.ylabel("Runtime (s)")
plt.title("Runtime vs Threads (N = 100,000,000)")
plt.xticks(threads)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("runtime_vs_threads.png", dpi=300)

# -----------------------------------
# Plot: Speedup vs Threads
# -----------------------------------

plt.figure()
plt.plot(threads, S_threaded, marker='o', label='std::thread')
plt.plot(threads, S_openmp, marker='o', label='OpenMP')
plt.xlabel("Number of Threads")
plt.ylabel("Speedup (T_serial / T_p)")
plt.title("Speedup vs Threads (N = 100,000,000)")
plt.xticks(threads)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("speedup_vs_threads.png", dpi=300)

# -----------------------------------
# Plot: Efficiency vs Threads
# -----------------------------------

plt.figure()
plt.plot(threads, E_threaded, marker='o', label='std::thread')
plt.plot(threads, E_openmp, marker='o', label='OpenMP')
plt.xlabel("Number of Threads")
plt.ylabel("Efficiency (Speedup / p)")
plt.title("Efficiency vs Threads (N = 100,000,000)")
plt.xticks(threads)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("efficiency_vs_threads.png", dpi=300)

print("Plots saved as:")
print("  runtime_vs_threads.png")
print("  speedup_vs_threads.png")
print("  efficiency_vs_threads.png")
