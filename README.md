# libasm
Beginner-friendly x86_64 assembly guide (Linux) with key/value tables for registers, flags, instructions, calling convention, and a project overview for the libasm assignment.

## Mental model in 5 lines
- CPU has a small set of registers (fast storage) and a flags register (status bits).
- Instructions move/transform data and update flags.
- Jumps look at flags to decide the next address to execute.
- Functions follow a calling convention (where args and return live).
- Memory is accessed by addresses; strings end with a byte 0 (`'\0'`).

## Registers (general-purpose)
| Key  | Value |
|------|-------|
| rax  | Accumulator and function return. Aliases: eax/ax/ah/al. |
| rbx  | Callee-saved general register. Aliases: ebx/bx/bh/bl. |
| rcx  | Counter for loops/shifts. Aliases: ecx/cx/ch/cl. |
| rdx  | Data for mul/div; 3rd syscall arg. Aliases: edx/dx/dh/dl. |
| rsi  | Arg2 in SysV; string source. Aliases: esi/si/sil. |
| rdi  | Arg1 in SysV; string dest / first pointer. Aliases: edi/di/dil. |
| rsp  | Stack pointer (top of stack). Aliases: esp/sp/spl. |
| rbp  | Frame/base pointer (if used). Aliases: ebp/bp/bpl. |
| r8   | Caller-saved temp. Aliases: r8d/r8w/r8b. |
| r9   | Caller-saved temp. Aliases: r9d/r9w/r9b. |
| r10  | Caller-saved temp. Aliases: r10d/r10w/r10b. |
| r11  | Caller-saved temp. Aliases: r11d/r11w/r11b. |
| r12  | Callee-saved. Aliases: r12d/r12w/r12b. |
| r13  | Callee-saved. Aliases: r13d/r13w/r13b. |
| r14  | Callee-saved. Aliases: r14d/r14w/r14b. |
| r15  | Callee-saved. Aliases: r15d/r15w/r15b. |

## Flags (RFLAGS) essentials
| Key | Value |
|-----|-------|
| ZF  | Zero Flag: 1 when result is zero (used by `je/jne`). |
| CF  | Carry Flag: carry/borrow for unsigned arithmetic; used by `jc/jb/jae/adc/sbb`. |
| SF  | Sign Flag: copy of the sign bit of the result. |
| OF  | Overflow Flag: signed overflow occurred. |
| PF  | Parity Flag: parity of low byte (rarely used). |
| AF  | Adjust Flag: BCD adjust (rare in modern code). |
| DF  | Direction Flag: string ops direction (0 = forward). |
| IF  | Interrupt Flag: mask external interrupts (kernel/boot use). |

## Core instructions (what they change)
| Key | Value |
|-----|-------|
| `mov dst, src` | Copy data; does not change flags. |
| `xor reg, reg` | Clear reg to 0; sets ZF=1, CF=0; breaks dep on old value. |
| `add/sub reg, imm/reg` | Arithmetic; updates ZF, SF, CF, OF, PF, AF. |
| `inc/dec reg` | +/-1; updates ZF, SF, OF, PF, AF; keeps CF unchanged. |
| `and/or/xor/not` | Bitwise; update ZF/SF/CF/OF accordingly (xor clears CF/OF). |
| `shl/shr/sar` | Shifts; CF gets last shifted-out bit, ZF/SF updated. |
| `cmp a, b` | Compute a-b for flags only; does not store the result. |
| `test a, b` | Bitwise AND for flags only; common for zero/non-zero checks. |
| `jmp label` | Unconditional jump; flags unchanged. |
| `je/jne/jl/jg/ja/jb ...` | Conditional jumps that read flags (ZF/CF/SF/OF). |
| `push/pop reg` | Stack ops: adjust `rsp`, store/load 8 bytes. |
| `call label` | Push return address, jump to label. |
| `ret` | Pop return address into RIP (return to caller). |

## Calling convention (Linux x86_64 SysV)
| Key | Value |
|-----|-------|
| Args order | rdi, rsi, rdx, rcx, r8, r9 |
| Return     | rax (scalars/pointers), xmm0 for float/double |
| Caller-saved | rax, rcx, rdx, rsi, rdi, r8–r11 |
| Callee-saved | rbx, rbp, r12–r15 (must be preserved by the callee) |
| Stack align  | Before `call`, `rsp` must be 16-byte aligned |

## Addressing cheat sheet
| Key | Value |
|-----|-------|
| `[reg]` | Memory at address in reg. |
| `[reg + imm]` | Memory at reg plus constant offset. |
| `[base + index*scale + disp]` | Full form; scale = 1,2,4,8. |
| `byte/word/dword/qword [..]` | Size hint for the load/store. |

## Jump conditions cheat sheet
| Key | Value |
|-----|-------|
| `je/jz` | Jump if ZF=1 (equal/zero). |
| `jne/jnz` | Jump if ZF=0 (not equal). |
| `ja` | Jump if CF=0 and ZF=0 (unsigned >). |
| `jb/jc` | Jump if CF=1 (unsigned <). |
| `jae/jnc` | Jump if CF=0 (unsigned ≥). |
| `jbe` | Jump if CF=1 or ZF=1 (unsigned ≤). |
| `jg` | Jump if ZF=0 and SF=OF (signed >). |
| `jl` | Jump if SF≠OF (signed <). |
| `jge` | Jump if SF=OF (signed ≥). |
| `jle` | Jump if ZF=1 or SF≠OF (signed ≤). |


## Minimal Hello World (syscall example)
```asm
section .data
msg:    db "Hello, world!", 10
len:    equ $ - msg

section .text
global _start

_start:
    mov     rax, 1          ; syscall write
    mov     rdi, 1          ; fd = stdout
    mov     rsi, msg        ; buf
    mov     rdx, len        ; count
    syscall                 ; write(1, msg, len)

    mov     rax, 60         ; syscall exit
    xor     rdi, rdi        ; status = 0
    syscall
```

## References
- Subject PDF: [en.subject.pdf](en.subject.pdf)
- SysV AMD64 ABI: <https://uclibc.org/docs/psABI-x86_64.pdf>