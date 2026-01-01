
global ft_write
extern __errno_location
section .text

ft_write:
    ; rdi = fd, rsi = buf, rdx = count
    mov     rax, 1              ; SYS_write
    syscall
    test    rax, rax            ; success if rax >= 0
    jns     .ok
    mov     edx, eax            ; edx = -errno
    neg     edx                 ; errno as positive
    sub     rsp, 8              ; align stack for call
    call    __errno_location wrt ..plt
    add     rsp, 8
    mov     [rax], edx          ; set errno
    mov     rax, -1             ; return -1 on error
.ok:
    ret

; Explanation:
; rdi = fd, rsi = buf, rdx = count
; returns: bytes written in rax, or -1 on error with errno set.
; 1) Issue SYS_write (rax = 1).
; 2) On success (rax >= 0) return rax.
; 3) On error: rax has -errno; store +errno via __errno_location and return -1.
