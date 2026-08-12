# C / Systems Experiments

A small archive of low-level C/CMake experiments created while exploring simulation, numerical code and native build tooling.

## Contents

- `cevolution/` — evolutionary / simulation experiments
- `clife/` — an early Game of Life implementation; the actively developed successor lives in the dedicated [`clife`](https://github.com/lisu188/clife) repository
- `cneuro/` — neural/network-oriented C experiments
- `cmake/` — shared CMake helpers and build experiments

## Build

The repository uses CMake. Individual subprojects may have different historical requirements.

```bash
cmake -S . -B build
cmake --build build
```

## Status

**Legacy experiment collection.** This repository is intentionally an engineering notebook rather than a maintained product. It remains public to show earlier native-code exploration; current systems/performance work is better represented by `clife`, `fall-of-nouraajd`, `clash-disassembly` and `clash-hd`.
