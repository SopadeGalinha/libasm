global ft_strcpy
section .text

ft_strcpy:
    mov     rdx, rdi            ; keep original dest for return
.loop:
    mov     al, [rsi]           ; load src byte
    mov     [rdi], al           ; store into dest
    inc     rsi                 ; advance src
    inc     rdi                 ; advance dest
    test    al, al              ; was it '\0'?
    jne     .loop               ; if not, keep copying
    mov     rax, rdx            ; return original dest
    ret

; Explanation:
; rdi = dest (argument 1)
; rsi = src  (argument 2)

; 1) Save original dest in rdx for the return value.
; 2) Loop: load byte from [rsi] into al; store al into [rdi]. 
; 3) increment(inc) src/dest pointers.
; 4) Check if the byte was not 0, continue.
; 5) When 0 is copied, move saved dest to rax and return.
