global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy
section .text

ft_strdup:
    push    r12                 ; save callee-saved reg for src (keeps alignment)
    mov     r12, rdi            ; r12 = src
    call    ft_strlen wrt ..plt ; rax = len(src)
    lea     rdi, [rax + 1]      ; rdi = len + 1 (include '\0')
    call    malloc wrt ..plt    ; rax = dst
    test    rax, rax            ; NULL?
    je      .fail
    mov     rdi, rax            ; dst -> rdi
    mov     rsi, r12            ; src -> rsi
    call    ft_strcpy wrt ..plt ; copy, returns dst in rax
    pop     r12
    ret
.fail:
    pop     r12                 ; unwind saved src; rax already NULL
    ret

; Explanation:
; Input:  rdi = src
; Return: duplicated string pointer (rax) or NULL.
; Steps: save src in r12, measure len, malloc len+1, copy via ft_strcpy.