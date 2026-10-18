# OS1 kernel

A small preemptive multitasking kernel for the Intel 8086, running under DOS.

- **Threads** with their own stacks and PCBs, a `waitToComplete()` join, and an idle thread
- **Time-sliced preemption** driven by the timer interrupt (IVT entry 8). Context switches happen in the interrupt handler.
- **Semaphores** with `wait(maxTimeToWait)` timeouts
- **Events** bound to arbitrary interrupt entries through `PREPAREENTRY`
- **Thread signals** with registered handlers that can be blocked and unblocked

```
h/    public API (thread.h, semaphor.h, event.h) and kernel internals (pcb.h, kersem.h, kernelev.h)
src/  the implementation
```

Built for the Operating Systems 1 course at ETF Belgrade. The project spec (2020, Serbian) is included as a PDF.

## Build

You need Borland C++ 3.1 in a DOS environment such as DOSBox. Link against `APPLICAT.LIB`, the course test harness, and run the resulting `.exe`.
