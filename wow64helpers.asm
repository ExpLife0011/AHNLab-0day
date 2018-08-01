.MODEL FLAT
.code
_ToLower PROC                        
			cmp ax,41h
			jge greater_equal
			ret
greater_equal:
			cmp ax, 5Ah
			jbe range
			ret
range:
			sub ax, 20h
			ret
_ToLower ENDP


_AsmWcslen PROC						 
			
			db 56h						; push rsi 

			db 48h 
			db 89h 
			db 0CEh						; mov    rsi, rcx
cont:
			lodsw
			test ax,ax
			jnz cont


			db 48h 
			db 29h 
			db 0ceh						; sub    rsi, rcx

			db 48h 
			db 89h 
			db 0f0h						; mov    rax, rsi

			db 48h 
			db 0ffh 
			db 0c8h						; dec    rax 

			db 48h 
			db 0d1h 
			db 0f8h						; sar    rax, 1

			db 5eh						; pop rsi
			ret
_AsmWcslen ENDP


_CompareIcase PROC

			db 53h						; push   rbx
			db 56h						; push   rsi
			db 57h						; push   rdi

next_letter:			

			db 66h 
			db 42h
			db 8bh  
			db 4 
			db 41h						 ; mov    ax, WORD PTR[rcx + r8 * 2]

			call _ToLower
            mov bx,ax
			db 66h 
			db 42h 
			db 8bh 
			db 4 
			db 42h						 ; mov    ax, WORD PTR[rdx + r8 * 2]
			
			call _ToLower

			cmp ax,bx
			jnz fail

			db 49h 
			db 0ffh 
			db 0c8h						 ; dec    r8
            jns next_letter

			db 0b8h 
			db 1 
			db 0 
			db 0 
			db 0						 ; mov    eax, 0x1
cleanup:

			db 5fh                       ; pop    rdi
			db 5eh                       ; pop    rsi
			db 5bh                       ; pop    rbx 
			ret
fail:
			xor eax,eax
			jmp cleanup

_CompareIcase ENDP

_AsmStrlen PROC
	db 56h							; push   rsi

	db 48h 
	db 89h 
	db 0ceh							; mov    rsi,rcx
next_sym:
	db 0ach							; lods   al,BYTE PTR ds:[rsi]
	
	db 84h 
	db 0c0h							; test   al,al 
	jnz next_sym
	db 48h 
	db 89h 
	db 0f0h							; mov    rax,rsi

	db 48h 
	db 29h 
	db 0c8h							; sub    rax,rcx

	db 48h 
	db 0ffh 
	db 0c8h							; dec    rax 

	db 5eh							; pop    rsi 
	ret
_AsmStrlen ENDP

_AsmStrcmp PROC
	db 56h							; push   rsi
    db 57h							; push   rdi 
	

	db 48h 
	db 89h 
	db 0ceh							; mov    rsi,rcx

	db 48h 
	db 89h 
	db 0d7h							; mov    rdi,rdx

	db 4ch 
	db 89h 
	db 0c1h							; mov    rcx,r8

	db 0f3h 
	db 0a6h							; repz cmps BYTE PTR ds:[rsi],BYTE PTR es:

	db 48h 
	db 89h 
	db 0c8h							; mov    rax,rcx 
	

	db 5fh							; pop    rdi
	db 5eh							; pop    rsi 
	ret
_AsmStrcmp ENDP
_AsmGetModule64NtHeader PROC
	db 48h 
	db 89h 
	db 0c8h							; mov    rax,rcx

	db 8bh 
	db 40h 
	db 3ch							; mov    eax,DWORD PTR [rax+0x3c]

	db 48h 
	db 01 
	db 0c8h							; add    rax,rcx
	ret
_AsmGetModule64NtHeader ENDP
END

