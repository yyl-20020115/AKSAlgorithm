global find_msb_position

; Input
;   rdi - long long to find msb

; Output
;   rax - msb position

find_msb_position :
	test rcx,rcx
	jz is_zero
	bsr rax, rcx
	ret
is_zero:
	xor rax,rax
	ret
