# Cliff notes

Introduction to HPC Scientific Computing, Eijkhout et al., 2014

## 1 - Single-processor Computing

### von Neumann
- fetch, execute, store (in-order, control-flow); RAM (pure, no locality effects), stored program

[Registers](https://godbolt.org/g/Hh2De5)
- de facto instanteneous
- 16-32 FP registers
- aim for register residency by explicit variables
- register spill if requesting more than available

### Pipelines
- different execution levels
- multiple independent instructions in flight at different stage

### Instruction level parallelism (ILP)
- discovered
- chances of validity decreases exponentially with instruction depth (branching, interdependence)
- from control-flow to data-flow; superscalar

Memory (bandwith, latency) vs CPU (rating)
bound

### [Cores](https://hw-lab.com/intel-sandy-bridge-cpu-microarchitecture.html/6)
- out-of-order (additional transistors, energy) vs in-order (Intel Xeon Phi) execution handling
- seperate units (addition, multiplication, fused multiply-add), duplicate functionality for ILP and out-of-order
- mult/add in one cycle asymptotically (see pipelining), while division up to 20 cycles

### Pipelining
- example: decode, find locations; copy to register; align exponents; add mantissa; normalize; store.
- with pipelining, mutltiple indepedent operations in fligth at different stage, means asymptotically (with enough instructions n_1/2), we can execute one per cycle
- longer pipelines (finer grained steps) can increase asymptotic speed but need more independent instructions in continuous stream. likelihood of independence and no branching decreases non-linear with number of instrucion pipeline
- peak performance: number of cores times number of FPU units times clock speed
  - unobtainable, linpack matrix multiplication gets up to 70%
- pipline compatible: vector addition. not: inner product, since we read and write `s`, so subsequent ops are not independent:
```
for (i = 0; i < N; i++) 
  s += a[i]*b[i]
```
- Loop unrolling tries to tailor to this
- [versions](https://godbolt.org/g/1iuQAM)
- multiplication instructions are independent due to temporary split sums, which reside in register
- addition can further be extracted, and multiplication assigned to temporary, also in cache
```
for (i = 0; i < N/2-1; i++) 
{
  sum1 += a[2*i] * b[2*i];
  sum2 += a[2*i+1] * b[2*i+1];
}
```

### 32-bit, 64-bit
- width of bus between processor and memory (64-bit can load one double per cycle)
- memory address size
- (integer) register size (floats bigger) = size of memory unit can operate on simultaneously

### ILP is facilitated by
- independent
- pipelined
- branch-prediction
- out-of-order execution
- prefetching

### Memory hierarchies
- up to 100-1000 cycles to fetch from RAM
- register, L1, L2 ... cache, RAM

### Busses
- wires through which data move between components
- Front-side bus (north bridge): CPU, AGP, RAM
- South bridge: bios, IO, USB, PCI
- around one GHz rating
- bandwidth (bus width) 64-128 bit per cycle

### Latency vs bandwidth
- memory stall, ns/cycles
- latency hiding: processing other/more instructions while waiting for memory, thread swithching (GPU strength)
- time to data = latency + 1/bandwith times data size

### Caches
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

### Concurrency
- To exploit memory bandwith, enought relevant and independent requests must be made per cycle
- Prefetching addresses this to some extent
- Little's law: concurrency = bandwith x latency, effective throughput = expressed concurrency/latency

### Memory banks
- Interleaved banks to allow for simultaneous fetch of adjacent/sequential data, ie 4 way

### TLB
- Translation Look-aside buffer
- fast, cached translation from program logical to physical memory pages, can miss

### Multicore architecture
- increasing clock frequency heats up chip and doesn't address memory wall
- ILP has logical limits
- move from detected ILP to explicit task parallelism
- Core
  - L1 cache, processing units, pipeline
- Socket/chip
  - Shared L2
  - Multiple cores (eg OpenMP)
- Node
  - Shared memory
  - One or more sockets
- Network
  - Multiple nodes

### Cache coherence
- Between core dedicated caches
- Always applies to entire cache line
- MSI: modified, shared (at least one cache and unmodified), invalid
- Snooping (detectin sharing of cache lines)
  - Listen to bus traffic
- False sharing
  - Data close in memory that are used by different cores
  - Since always entire cache line is tainted, coherence is an issue even though the same data is not touched

### Locality
- Data used in related operations stored closely in memory
- Arithmetic intensity
  - Operations per data (addidion: one op, two accesses)
  - Favorable for matrix-matrix mult (neural networks)
  - Roofline model: performance vs arithmetic intensity, compute bound vs memory bound
- Temporal locality
  - Pertains to algorithmic distance of instructions that use same data
- Spatial locality
  - Pertains to layout of data in memory
- Core locality
  - Pertians to task based parallelism, threads, processing unit, writing to same memory from different threads

### Programming Strategies
- Goedeker and Hoisie
- Loop tiling (cache blocking)
```
for (b=0; b<size/l1size; b++) 
{
  blockstart = 0;
  for (i=0; i<NRUNS; i++) {
    for (j=0; j<l1size; j++)
      array[blockstart+j] = 2.3*array[blockstart+j]+1.2;
}
```
- Cache oblivious programming for portability
- Most important message seems to be use of cache lines, ie locality
- Loop unrolling, see above and
```
for (i)
  for (j)
    y[i] = y[i] + a[i][j] * x[j];
```
becomes
```
for (i=0; i<M; i+=2) 
{
  s1 = s2 = 0;
  for (j) 
  {
    s1 = s1 + a[i][j] * x[j];
    s2 = s2 + a[i+1][j] * x[j];
  }
  y[i] = s1; y[i+1] = s2;
}
```
- Loop unrolling limited by number of registers (16-32)
- Lower order operations (eg copying) cost is amortizible

### Power consumption
- Increases with component count
- Decreases with component size about proportioinally (voltage, current)
- Frequency increase leads to heat production
- Miniaturization and voltage lowering yields leakage eventually
- Multicore approach
- Several megawatts for parallel computer

### Unix process
- execution of single program 
- in memory
  - program code in machine instructions
  - heap - malloc
  - stack - local items, stack/frame pointer and program counter, intermediate results
- own data space

## 2 - Parallel Computing

- Hard to define parallelism, many levels (ILP, threads, processes)
- Parallel computer: multiple processors on same problem
- Concurrency is the composition of independently executing processes
- Parallelism is the simultaneous execution of (possibly related) computations
- Goes on to explain map reduce example, communication cost, embarassingly parallelism and logarithmic scaling
- Speedup us single/multiple processor runtime ratio - a better definition is best single processor implementation of a problem vs parallel implementation
- Efficiency is speedup over number of processors, obviously < 1 because of communication, load balancing, sequential code paths
- This definition suffers with problem sizes exceeding single machine capacity
- On a strict von Neuman model superlinear speedup is impossible, but memory effects can lead to this, and always if disc swapping can be prevented
- Critical path: dependencies between operations which limit parallelism, sequential limitations and communication

### Amdahl's law
- Code parts that are inherently sequential limit speedup
- Possible speedup is less than inverse of serial fraction of execution time
- Efficiency decreases proportionally to number of processors
- If one accounts for communication overhead, which increases proportionally to number of processors, this is even worse
- The number of processors should not grow beyond the ratio of scalar execution time and communication overhead
- Amdahl's law assumes same computational effort on all processors - which decreases in most applications
- To account for this, it is more realistic to look at the sequential fraction independent of problem size
- In hybrid parallelization, speedup is limited by the task parallel portion over the serial portion

### Scalability
- *Strong* scalability indicates processor count up to which speedup is maintained
- *Weak* scalability describes problems where the the speed of operations per processor remains constant as the problem size grows such thath the amound of data per processor remains constant with increasing number of processors
- Some algorithms scale badly because of increased communiction overhead and data duplicatoin

### Computer Architecture
- Flynn's taxonomy: SISD (single CPU), SIMD (vectorization, threading eesh, GPU), MISD (defacto nonexistant), MIMD (parallel computers), where instruction refers to control flow
- Vector instructions SIMD Streaming Extensions, Advanced Vector Extensions (512 bit, 8 floats)
- Most supercomputers these days use commodity processors

### Memory Access
- distributed vs shared, address space
- Uniform Memory Access to abstract away physical boundaries of memory (aka Symmetric Multi Processing) - on one machine, connected through bus, uniform access time, shared L2 also an example
- Non-Uniform Memory Access, through exchange network, maintains single address space, no uniform access time - may be extended to network (virtual shared memory)
- Logically distributed memory, processors have own address space and no direct cross access, explicit communication - best scalability, hardest to program

### Granularity of Parallelism
- Data/fine-grained parallelism
  - Simple loop parallelization, candidates for vectorization, splitting up data structure, single instruction etc...
  - Efficient in lockstep parallelization accross multiple processors, think GPU
- Instruction-level parallelism
  - individual, similar, independent instructions in sequence
  - compiler job
- Task-level parallelism
  - subprogram identification
  - tree search
  - think threading, OpenMP, SIMD for multi instruction plus logic operations (tasks)
  - shared memory
- Embarassing parallelism
  - pure SIMD, single instruction, independent
  - aka convenient parallelism
- Medium grained parallelism
  - single loop with range partitioned according to number of processors
- Task granularity
  - size of tasks before synchronization
  - coarse: many instructions

### Parallel programming
- Thread parallelism
  - Think OpenMP: hints at parallelization points, promise of independence
  - A process can have multiple threads
  - Shared memory parallelization: a thread has access to all process data (heap), but can have its own stack
  - Threads are dynamic (can be created during program execution) - contrast MPI
  - Master thread starts, others spawned, master waits for completion - fork-join
  - One thread per core common model, multi-core processors
  - Some cores support multiple threads, ie through scheduling while one waits for data
  - Thread switchin expensive (except hyperthreading) on CPU, not on GPU
  - Thread spawning incurrs overhead - trade-off
  - Lock/Mutex (mutually exclusive) to avoid read-write collisions
  - Shared and private state to threads makes context
  - Context switch happens when more threads than cores and processor has to switch
  - Thread safety - same behaviour serial and parallel
  - Critical section: updates of shared state - e.g. use semaphore (lock indicator), thread sets, updates state, and unsets, others read and wait. or use lock on data structure itself. locks at os level slow
  - transactional memory - retry update if conflict detected - may incur pipeline flushing and cache invalidation
  - affinity: fixed mapping between threads/processes and cores, eg subsequent threats on same data should be on same core
  - sequential consistency: same result parallel/serial
  - hyperthreading runs threads truly simultaneously
  - multithreading has hardware supported storage for information of multiple threads, allows for fast switching (gpu, xeon phi)
  
### OpenMP
- threads, dynamic parallelis
- parallel regions
- facilitates incremental parallelization
- static scheduling can reuse data between subsequent but loose efficiency, dynamic the opposite
- chunk size vs false sharing

### MPI
- distributed memory, coordination via explicit messages
- blocking messages, cycle of waiting on depencies - deadlock
- ops: process management, p2p comm, collective calls
- buffer management (non-blocking comms) and blocking semantics central issues
- collective: reduction, broadcast, gather, scatter, all-to-all
- one-sided comm: eg remote-direct-memory-access (RDMA) suppor, declare shared area of memory available for one-sided transfer
- non-blocking need explicit check for completion

### Hybrid shared/distributed
- MPI uses copy if on same node
- non-obvious performance trade-offs

### Parallel languages
- Abstract level description of ops, distribution under hood
- HPF, Chapel, Unified Parallel C, Linda
- Partitioned Global Address Space
- may lead to inefficient code through convenience (?)
- most important aspect data treatment

### Bulk synchronous parallelism
- middle ground between message passing and complete abstractin
- explicit communication, implicit ordering
- all comms are one-sided and asynchronous
- supersteps, ending in barrier which assures synchronization
- Pregel lends from it

### Program design
- array-of-structures (messages) vs structures-of-array (SIMD)
- latency hiding, overlapping computation with (asynchronous) communication

### Topologies
- scheme to connect processors in parallel machine
- all-to-all and one-line ethernet don't scale obviously
- connecitvity is graph: degree, diameter and bandwith (bisection bandwith - simultaneous independent messages)
- busses: only one processor at a time can access memory, cache coherence and snooping
- linear arrays, rings
- cartesian arrangement, torous connections, 2D and 3D
- extended to hypercube: n-dim, 2**n nodes, each connected to n others
- switched network: crossbar, butterfly, fat tree
- affect latency and bandwith of cluster
  - latency: handshake, coding/decoding, transmission
  - bandwith: channel rate
- locality: between cores (private and shared cache), between sockets, between nodes
  
### Multi-threaded architecture
- More data parallelism than cores
- Switching between tasks while waiting on memory (context switch)
- Aimed at making context switch cheap
- coprocessors need explicit seperate streams of work
- coprocessors don't talk to the network directly
- GPU
  - combine SIMD and SPMD
  - threads not completely independent, ordered in thread blocks, all threads in a block execute same instruction, i.e. SIMD
  - instruction stream: *kernel* in CUDA, e.g. function
  - SIMT
  - GPU has several (dozens) of Streaming Multiprocessors (SMs), each with e.g. 8 streaming processors (cores)  
  - data parallel
  - threads within block share address space
  - truly parallel: conditional that leads to one branch pauses all other threads not in this branch etc...
  - data to GPU over bus, eg PCI-X, overhead
  - single precision emphasis, not ideal with branching
- Intel Xeon Phi, many integrated cores, SIMD style, bus connected, general purpose cores
