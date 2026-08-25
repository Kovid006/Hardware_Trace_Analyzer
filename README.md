# Hardware Trace Analyzer

A C++ program for analyzing hardware memory access trace files.

## Features

- Load hardware trace files
- Display trace entries
- Read/write access breakdown
- Address access frequency analysis
- Most frequently accessed address
- CPU-wise access breakdown
- Timing analysis
- Average gap between memory accesses
- Input validation for menu selection

## Trace Format

The analyzer expects each trace entry in the following format:

```text
TIME CPU OPERATION ADDRESS
