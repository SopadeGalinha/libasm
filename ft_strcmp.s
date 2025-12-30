global ft_strcmp
section .text

ft_strcmp:
.loop:
	movzx   eax, byte [rdi]     ; load s1 byte (zero-extend)
	movzx   edx, byte [rsi]     ; load s2 byte (zero-extend)
	cmp     eax, edx            ; compare bytes
	jne     .diff               ; if differ, return diff
	test    eax, eax            ; check for '\0'
	je      .done               ; both zero → equal
	inc     rdi                 ; advance s1
	inc     rsi                 ; advance s2
	jmp     .loop
.diff:
	sub     eax, edx            ; (unsigned)s1 - (unsigned)s2
	ret
.done:
	xor     eax, eax            ; If strings are equal, return 0
	ret
