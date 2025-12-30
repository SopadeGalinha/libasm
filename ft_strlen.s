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


; Registers Used:
;   RDI: Points to the input string.
;   RAX: Stores the string length (output)