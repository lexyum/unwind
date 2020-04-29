# unwind

A simple brainfuck interpreter written in C, implementing loop structures with a stack. Very minimal, data section of 30000 byte cells with no internal bounds checking (so don't overrun it!).

The example programs `hello.bf` and `fib.bf` are including for checking: the latter does work, but quickly starts overflowing data cells.
