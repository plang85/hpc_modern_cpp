# Cliff notes

Introduction to HPC Scientific Computing, Eijkhout et al., 2014

## 1 - Single-processor Computing

von Neumann
- fetch, execute, store (in-order, control-flow); RAM (pure, no locality effects), stored program

[Registers](https://godbolt.org/g/Hh2De5)
- de facto instanteneous
- 16-32 FP registers
- aim for register residency by explicit variables


Pipelines
- different execution levels
- multiple independent instructions in flight at different stage

Instruction level parallelism (ILP)
- discovered
- chances of validity decreases exponentially with instruction depth (branching, interdependence)
- from control-flow to data-flow; superscalar

Memory (bandwith, latency) vs CPU (rating)
bound

[Cores](https://hw-lab.com/intel-sandy-bridge-cpu-microarchitecture.html/6)
- out-of-order (additional transistors, energy) vs in-order (Intel Xeon Phi) execution handling
- seperate units (addition, multiplication, fused multiply-add), duplicate functionality for ILP and out-of-order
- mult/add in one cycle asymptotically (see pipelining), while division up to 20 cycles

Pipelining
- example: decode, find locations; copy to register; align exponents; add mantissa; normalize; store.
- with pipelining, mutltiple indepedent operations in fligth at different stage, means asymptotically (with enough instructions n_1/2), we can execute one per cycle
- longer pipelines (finer grained steps) can increase asymptotic speed but need more independent instructions in continuous stream. likelihood of independence and no branching decreases non-linear with number of instrucion pipeline
- peak performance: number of FPU units times clock speed

32-bit, 64-bit
- width of bus between processor and memory (64-bit can load one double per cycle)
- memory address size
- (integer) register size (floats bigger) = size of memory unit can operate on simultaneously

ILP is facilitated by
- independent
- pipelined
- branch-prediction
- out-of-order execution
- prefetching

Memory hierarchies
- up to 100-1000 cycles to fetch from RAM
- register, L1, L2 ... cache, RAM

Busses
- wires through which data move between components
- Front-side bus (north bridge): CPU, AGP, RAM
- South bridge: bios, IO, USB, PCI
- around one GHz rating
- bandwidth (bus width) 64-128 bit per cycle

Latency vs bandwidth
- memory stall, ns/cycles
- latency hiding: processing other/more instructions while waiting for memory, thread swithching (GPU strength)
- time to data = latency + 1/bandwith times data size

Caches
- L1..LX SRAM
- Bandwidth 32 bytes L1, same for L2 and L3, but there some goes towards coherence permanently
- DRAM bandwidht 4.5 bytes shared between cores
and sockets (chips)
- Misses
  - compulsory (never requested)
  - capacity (even ideal cache strategy not able to keep up)
  - conflict (better candidates for eviction, strategy victim)
  - invalidation (multicore transaction conflict)
