# 02 — Inline Assembly: Constraints & Special Registers

Bare-metal ARM inline assembly on STM32F407. Explores how C variables bind to
assembly operands through constraint strings, and how special (core) registers
are read with MRS.

## What it demonstrates
- **`"r"` (input, register)**: `MOV R0, %0` with `"r"(val)` — compiler loads the C
  variable into a general-purpose register.
- **`"i"` (immediate)**: same MOV with `"i"(0x50)` — the value is encoded directly
  into the instruction, no register used. (Note: this second MOV overwrites R0;
  both lines exist only to contrast register vs immediate.)
- **`"=r"` (output, write-only)**: `MRS %0, CONTROL` reads the special CONTROL
  register into a C variable. Special registers require MRS/MSR, not MOV; the
  `=` modifier marks the operand as write-only.
- **Input + output together**: `MOV %0, %1` with `"=r"(var1)` and `"r"(var2)`
  copies var2 into var1. Operands are numbered output-first: `%0`=var1, `%1`=var2.

## Key idea: constraints are the C ↔ assembly bridge
`"r"` / `"=r"` tell the compiler to move C variables in and out of registers
automatically. A single MOV expands (in disassembly) to LDR → MOV → STR, because
the variables live in RAM and ARM's load/store architecture requires arithmetic
to happen in registers.

## Verification
Compiled and inspected the disassembly; traced R0 and the output variables in the
debugger (Registers view) on the STM32F407G-DISC1.

## Türkçe
STM32F407 üzerinde inline assembly alıştırması. Constraint string'lerin (`"r"`,
`"=r"`, `"i"`) C değişkenleriyle assembly operandlarını nasıl bağladığını ve özel
(core) register'ların (CONTROL) MRS ile nasıl okunduğunu gösterir. Tek bir MOV'un
arka planda LDR → MOV → STR'ye açılması (load/store mimarisi) disassembly'de
incelenmiştir.