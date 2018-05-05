# `time(1)` for DOS

Because the Internet apparently forgot every version of this tool that must
have been written over the years.

Currently supporting only IBM-compatibles, where it is implemented in terms of
the Intel 8253 PIT with its tick frequency of ⅓ × 315/88 ≈ 1,193181818… MHz.
PC-98 support would be nice to have.

## Building

Currently, this code only supports Borland's old DOS compilers. Version ≥4.0 of
Turbo C++ is required.

Simply run `MAKE` in the root directory to build `TIME.EXE`.
