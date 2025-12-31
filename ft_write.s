global ft_write
extern __errno_location
extern signal
section .text

ft_write:
    ; rdi = fd, rsi = buf, rdx = count
    push    r12                 ; save callee-saved + align stack (entry rsp%16 = 8)
    push    r13
    push    r14                 ; now rsp%16 = 0 before calls
    mov     r12, rdi            ; stash args across signal
    mov     r13, rsi
    mov     r14, rdx
    mov     rdi, 13             ; SIGPIPE
    mov     rsi, 1              ; SIG_IGN
    call    signal wrt ..plt    ; ignore SIGPIPE so syscall returns -EPIPE instead of killing
    mov     rdi, r12            ; restore args
    mov     rsi, r13
    mov     rdx, r14
    mov     rax, 1              ; SYS_write
    syscall
    test    rax, rax            ; success if rax >= 0
    jns     .ok
    mov     edx, eax            ; save -errno
    neg     edx                 ; errno > 0
    call    __errno_location wrt ..plt
    mov     [rax], edx          ; set errno
    mov     rax, -1             ; return -1 on error
.ok:
.cleanup:
    pop     r14
    pop     r13
    pop     r12
    ret
