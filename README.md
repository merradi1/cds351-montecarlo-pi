# cds351-montecarlo-pi
Monte Carlo Estimation of π — CDS 351 Team Project
# Monte Carlo π Estimation — CDS 351 Final Project

## Team Members
- Reda Erradi
- Mohamed Sharif

---

This project estimates π using a Monte Carlo method in three modes:
- Serial
- C++ std::thread
- OpenMP

The mode is selected using constants at the top of `par_mc.cpp`:  0 = serial, 1 = threaded, 2 = OpenMP

The program automatically runs three consecutive executions of the selected mode and reports the average π value and average runtime to provide more stable and reliable results.


Files:
par_mc.cpp — main implementation

Makefile — compile with make

run_serial.sh — run serial mode

run_threaded.sh — run std::thread mode

run_openmp.sh — run OpenMP mode

