# Cliff notes

Introduction to HPC Scientific Computing, Eijkhout et al., 2014

## 1 - Single-processor Computing

von Neumann
- fetch, execute, store; RAM (pure, no locality effects), stored program

[Registers](https://godbolt.org/g/Hh2De5)
- de facto instanteneous

Pipelines
- different execution levels
- multiple independent instructions in flight at different stage

Instruction level parallelism (ILP)
- discovered
- chances of validity decreases exponentially with instruction depth (branching, interdependence)

Memory (bandwith, latency) vs CPU (rating)
bound

[Cores](https://hw-lab.com/intel-sandy-bridge-cpu-microarchitecture.html/6)
- out-of-order (additional transistors, energy) vs in-order (Intel Xeon Phi) execution handling
- seperate units (addition, multiplication, fused multiply-add), duplicate functionality for ILP and out-of-order
- mult/add in one cycle asymptotically (see pipelining), while division up to 20 cycles

Pipelining
- example: decode, find locations; copy to register; align exponents; add mantissa; normalize; store.
- with pipelining, mutltiple indepedent operations in fligth at different stage, means asymptotically (with enough instructions n_1/2), we can execute one per cycle

32-bit, 64-bit
- width of bus between processor and memory (64-bit can load one double per cycle)
- memory address size
- (integer) register size (floats bigger) = size of memory unit can operate on simultaneously
