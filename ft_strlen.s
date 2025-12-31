global ft_strlen
section .text

ft_strlen:
    xor     rax, rax            ; Set RAX (len) to 0
.loop:
    cmp     byte [rdi + rax], 0 ; compare s[len] to '\0'
    je      .end                ; if zero, stop
    inc     rax                 ; len++
    jmp     .loop               ; repeat
.end:
    ret                         ; return len in rax


; Explanation:
; rdi = string;
; rax = counter/offset and return length.
; 1) Initialize rax = 0 (length/offset).
; 2) Loop: compare byte at [rdi + rax] to 0.
; 3) If not zero, inc rax and repeat.
; 4) If zero, return rax (length).