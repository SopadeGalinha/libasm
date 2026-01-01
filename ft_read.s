global ft_read
extern __errno_location
section .text

ft_read:
    ; rdi = fd, rsi = buf, rdx = count
    mov     rax, 0              ; SYS_read
    syscall                     ; rax = bytes read, or -errno
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
; Inputs: rdi=fd, rsi=buf, rdx=count
; Returns: bytes read (rax) or -1 with errno set from the kernel’s result.
; Behavior is identical to read(2): syscall handles EBADF/EFAULT/count==0 ordering.
