global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy
section .text

ft_strdup:
    push    rdi                 ; save src so we can reuse it after malloc
    call    ft_strlen wrt ..plt ; rax = len of src
    lea     rdi, [rax + 1]      ; rdi = len + 1 (size for malloc, with '\0')
    call    malloc wrt ..plt    ; rax = pointer to allocated memory
    test    rax, rax            ; check if malloc returned NULL
    je      .fail               ; if so, goes to .fail
    mov     rdi, rax            ; rdi = dst (arg1 for ft_strcpy)
    pop     rsi                 ; rsi = src (arg2 for ft_strcpy)
    call    ft_strcpy wrt ..plt ; copies src -> dst, returns dst in rax
    ret
.fail:
    add     rsp, 8              ; unwind saved src, keep rax (NULL) as return
    ret

; Explanation:
; Input:  rdi = src (argument 1)
; Return: pointer to duplicated string in rax, or NULL if malloc fails.
; Steps:
; 1) Save src on the stack; call ft_strlen(src) to get length.
; 2) Call malloc(len + 1); on NULL, return NULL.
; 3) Restore src into rsi.
; 4) set rdi to the allocated dst.
; 5) call ft_strcpy(dst=rdi, src=rsi).
; 6) ft_strcpy returns dst in rax
; 7) ret returns it to the caller.