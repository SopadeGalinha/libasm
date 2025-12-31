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

## Flags (RFLAGS): meaning and when they change
| Flag | Meaning | Set/Cleared by |
|------|---------|----------------|
| ZF (Zero) | 1 if result is zero | `cmp`, `test`, arithmetic/logic; **not** by `mov` |
| CF (Carry) | Unsigned carry/borrow | `add/sub`, shifts; cleared by `xor reg,reg`; **not** by `inc/dec` |
| SF (Sign) | Copy of MSB of result | Arithmetic/logic; tracks sign bit |
| OF (Overflow) | Signed overflow | Arithmetic on signed values (e.g., `add`, `sub`, `imul`) |
| PF (Parity) | Parity of low byte | Updated by arithmetic/logic (rarely used) |
| AF (Adjust) | BCD carry between nibble | Updated by `add/sub` (rarely used) |
| DF (Direction) | String op direction | Set/cleared by `std`/`cld` (0 = forward) |
| IF (Interrupt) | Mask external interrupts | `sti`/`cli` (kernel/boot code) |

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

## Conditional jumps (what flags they read)
| Jump | Condition | Flags used |
|------|-----------|------------|
| `je/jz` | equal / zero | ZF=1 |
| `jne/jnz` | not equal | ZF=0 |
| `ja` | unsigned > | CF=0 && ZF=0 |
| `jb/jc` | unsigned < | CF=1 |
| `jae/jnc` | unsigned ≥ | CF=0 |
| `jbe` | unsigned ≤ | CF=1 \|\| ZF=1 |
| `jg` | signed > | ZF=0 && SF=OF |
| `jl` | signed < | SF≠OF |
| `jge` | signed ≥ | SF=OF |
| `jle` | signed ≤ | ZF=1 \|\| SF≠OF |

## How common instructions affect flags
| Instr | ZF (zero) | CF (carry) | SF (sign) | OF (overflow) | Notes |
|-------|-----------|------------|-----------|---------------|-------|
| `mov dst, src` | — | — | — | — | Flags unchanged |
| `cmp a, b` | 1 if a==b | borrow from a-b | sign of (a-b) | signed overflow on (a-b) | No result stored |
| `test a, b` | 1 if (a&b)==0 | 0 | sign of (a&b) | 0 | AND for flags only |
| `add dst, src` | result==0 | carry out | sign of result | signed overflow | PF/AF also updated |
| `sub dst, src` | result==0 | borrow | sign of result | signed overflow | PF/AF also updated |
| `inc dst` | result==0 | **unchanged** | sign of result | signed overflow | CF not touched |
| `dec dst` | result==0 | **unchanged** | sign of result | signed overflow | CF not touched |
| `and/or/xor dst, src` | result==0 | 0 | sign of result | 0 | Logical ops clear CF/OF |
| `shl/shr dst, k` | result==0 | last bit shifted | sign of result | undefined for many counts | Use `sar` for signed right |
| `sar dst, k` | result==0 | last bit shifted | sign of result | undefined for many counts | Arithmetic right (keeps sign) |


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