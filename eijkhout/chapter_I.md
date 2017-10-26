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
  - only important if miss stalls processor
- Replacement stratgies
  - LRU (least recently used)
  - FIFO
- Lines
  - Data chunk moved between memory levels
  - 64-128 bytes
  - Use proper striding when accessing, follow memory outline, exploit locality
  - alignment considerations
- Mapping strategies
  - direct: quick calculation, e.g. 2 memory locations map to only one cache location
  - n-way associative: flexible, 1 memory location can map to n cache locations
- Hardware prefetch algorithms to reduce stalling

Concurrency
- To exploit memory bandwith, enought relevant and independent requests must be made per cycle
- Prefetching addresses this to some extent
- Little's law: concurrency = bandwith x latency, effective throughput = expressed concurrency/latency

Memory banks
- Interleaved banks to allow for simultaneous fetch of adjacent/sequential data, ie 4 way

TLB
- Translation Look-aside buffer
- fast, cached translation from program logical to physical memory pages, can miss

Multicore architecture
- increasing clock frequency heats up chip and doesn't address memory wall
- ILP has logical limits
- move from detected ILP to explicit task parallelism
- Core
  - L1 cache, processing units, pipeline
- Socket/chip
  - Shared L2
  - Multiple cores (eg OpenMP)
- Node
  - One or more sockets
- Network
  - Multiple nodes

Cache coherence
- Between core dedicated caches
- Always applies to entire cache line
- MSI: modified, shared (at least one cache and unmodified), invalid
- Snooping (detectin sharing of cache lines)
  - Listen to bus traffic
- False sharing
  - Data close in memory that are used by different cores
  - Since always entire cache line is tainted, coherence is an issue even though the same data is not touched

Locality
- data used in related operations stored closely in memory
- Arithmetic intensity
  - Operations per data (addidion: one op, two accesses)
  - Favorable for matrix-matrix mult (neural networks)