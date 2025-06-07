# MMSK Queue Simulation

## Overview
This project implements a simulation of an M/M/s/k queueing system in C. The system models a queue with `s` servers and a finite capacity of `k` customers, using Poisson arrival and service times. The simulation calculates key performance metrics such as average number of customers in the system (L), average number in queue (Lq), average time in system (W), and average time in queue (Wq), both through simulation and analytical formulas.

## Files
- **MMSK.c**: Main simulation program that models the M/M/s/k queue, simulates customer arrivals and departures, and computes performance metrics.
- **lcgrand.h**: Header file implementing a linear congruential random number generator for generating Poisson-distributed interarrival and service times.

## Features
- Simulates an M/M/s/k queue with configurable parameters:
  - `lambda`: Arrival rate (customers per hour).
  - `mu`: Service rate (customers per hour).
  - `S`: Number of servers.
  - `K`: System capacity (including those in service and queue).
  - `n`: Number of customers to simulate (default: 1,000,000).
- Computes performance metrics:
  - **L**: Average number of customers in the system.
  - **Lq**: Average number of customers in the queue.
  - **W**: Average time a customer spends in the system (in minutes).
  - **Wq**: Average time a customer spends in the queue (in minutes).
- Outputs results to both console and a CSV file (`mmsk.csv`) for further analysis.
- Includes analytical calculations for comparison with simulation results using steady-state queueing theory formulas.

## Prerequisites
- A C compiler (e.g., GCC).
- Standard C libraries (`stdio.h`, `stdlib.h`, `math.h`, `string.h`).

## Compilation and Execution
1. Ensure both `MMSK.c` and `lcgrand.h` are in the same directory.
2. Compile the program using a C compiler:
   ```bash
   gcc MMSK.c -o mmsk -lm
   ```
3. Run the executable:
   ```bash
   ./mmsk
   ```
4. The program will output performance metrics to the console and append them to `mmsk.csv` in the format:
   ```
   L_sim,Lq_sim,W_sim,Wq_sim,L_math,Lq_math,W_math,Wq_math
   ```

## Code Structure
- **Data Structures**:
  - `struct node`: Represents a customer with arrival time, service time, departure time, and a pointer to the next customer.
  - `struct hd`: Tracks server status (used or free) and departure time.
  - `struct Queue`: Represents the queue of waiting customers.
- **Key Functions**:
  - `poisson_intertime`: Generates Poisson-distributed interarrival or service times using the random number generator from `lcgrand.h`.
  - `getarrivalpacketinfo`: Creates a linked list of `n` customers with random arrival and service times.
  - `arrive`: Handles customer arrivals, assigning them to free servers or the queue.
  - `departure`: Processes customer departures, updating server status and queue.
  - `P0`, `Pn`, `Lq_math`, `L_math`: Calculate analytical performance metrics based on M/M/s/k queueing theory.
- **Random Number Generator**:
  - `lcgrand.h` provides a linear congruential generator for reproducible random numbers, with functions to generate, set, and get seeds.

## Parameters
The default parameters in `MMSK.c` are:
- `lambda = 25.0` (arrivals per hour).
- `mu = 40.0` (services per hour).
- `S = 2` (servers).
- `K = 8` (system capacity).
- `n = 1,000,000` (number of customers simulated).

To modify these, edit the `main` function in `MMSK.c`.

## Output
The program outputs:
- Simulation-based metrics: `L`, `Lq`, `W`, `Wq`.
- Analytical metrics: `L_math`, `Lq_math`, `W_math`, `Wq_math`.
- Results are printed to the console and saved to `mmsk.csv` for analysis.

## Notes
- The simulation assumes a stable system (i.e., `lambda / (S * mu) < 1`).
- The `lcgrand.h` random number generator uses a fixed seed for reproducibility. Modify `zrng` in `lcgrand.h` for different random sequences.
- The program assumes `INT_MAX` for initial server departure times to indicate idle servers.
- The queue is implemented as an array of pointers to handle up to `K` customers.

## Limitations
- The random number generator in `lcgrand.h` is basic and may not be suitable for all applications requiring high-quality randomness.
- The simulation assumes exponential interarrival and service times (M/M/s/k model).
- Large values of `n` may increase runtime and memory usage due to the linked list of customers.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
