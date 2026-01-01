global ft_read
extern __errno_location
section .text

ft_read:
    ; Short-circuit trivial/error cases before issuing the syscall
    test    rdx, rdx            ; count == 0 ?
    je      .zero_ret           ; POSIX: return 0 immediately when count is zero
    test    rsi, rsi            ; buf == NULL ?
    jne     .syscall_read
    sub     rsp, 8              ; keep stack 16B-aligned for the call
    call    __errno_location wrt ..plt
    add     rsp, 8
    mov     dword [rax], 14     ; EFAULT for NULL buffer
    mov     rax, -1
    ret

.syscall_read:
    mov     rax, 0              ; SYS_read number
    syscall                     ; rax = bytes read, or -errno on failure
    test    rax, rax            ; success path when rax >= 0
    jns     .ok
    mov     edx, eax            ; edx = -errno (low 32 bits are enough)
    neg     edx                 ; errno as positive
    sub     rsp, 8              ; realign stack for call
    call    __errno_location wrt ..plt
    add     rsp, 8
    mov     [rax], edx          ; *errno = errno
    mov     rax, -1             ; function return value on error
.ok:
    ret

.zero_ret:
    xor     eax, eax            ; count == 0 -> return 0
    ret

; Explanation:
; Inputs:
;   rdi=fd,
;  rsi=buf,
;  rdx=count
; Return: bytes read (rax) or -1 with errno set.
; Flow:
; 1) If count == 0 -> return 0 immediately (no syscall).
; 2) If buf is NULL and count > 0 -> set errno=EFAULT, return -1.
; 3) Otherwise issue SYS_read (rax=0).
;    - On success (rax >= 0) return rax.
;    - On error (rax < 0) store +errno via __errno_location and return -1.

; Quick standalone test (no lib build needed):
;   main.c:
;     #include <errno.h>
;     #include <fcntl.h>
;     #include <stdio.h>
;     #include <stdlib.h>
;     #include <unistd.h>
;     ssize_t ft_read(int fd, void *buf, size_t count);
;     int main(void) {
;         int fd = open("foo.txt", O_RDONLY);
;         if (fd < 0) return perror("open"), EXIT_FAILURE;
;         char b[1024]; ssize_t n;
;         while ((n = ft_read(fd, b, sizeof b)) > 0) write(1, b, (size_t)n);
;         if (n < 0) perror("ft_read");
;         close(fd); return n < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
;     }
; Build/run:
;   nasm -f elf64 ft_read.s -o ft_read.o
;   gcc main.c ft_read.o -o demo
;   ./demo   # assumes foo.txt exists
