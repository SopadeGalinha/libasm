global ft_strdup
extern malloc
extern ft_strlen
section .text

ft_strdup:
    ; rdi = src
    push    rdi                 ; save src
    call    ft_strlen wrt ..plt ; rax = len
    mov     rsi, rax            ; rsi = len
    inc     rsi                 ; +1 for '\0'
    mov     rdi, rsi            ; malloc(len+1)
    call    malloc wrt ..plt
    test    rax, rax            ; NULL?
    je      .fail
    pop     rdi                 ; restore src into rdi
    mov     rdx, rax            ; keep dst in rdx for return
.copy:
    mov     cl, [rdi]
    mov     [rax], cl
    inc     rdi
    inc     rax
    test    cl, cl
    jne     .copy
    mov     rax, rdx            ; return dst
    ret
.fail:
    add     rsp, 8              ; pop saved src
    ret