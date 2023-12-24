.code
SwapY proc
	vmovaps ymm0, ymmword ptr [rcx]
	vmovaps ymm1, ymmword ptr [rdx]
	vmovaps ymmword ptr [rdx], ymm0
	vmovaps ymmword ptr [rcx], ymm1
	ret
SwapY endp

; extern "C" void CopyY(void* dst, void* src, size_t r_size)                     
CopyY proc
	xchg rcx, r8
	shr rcx, 5
	mov rax, 32
@@:
	vmovaps ymm0, ymmword ptr [rdx]
	vmovaps ymmword ptr [r8], ymm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymm0, ymmword ptr [rdx]
	vmovaps ymmword ptr [r8], ymm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymm0, ymmword ptr [rdx]
	vmovaps ymmword ptr [r8], ymm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymm0, ymmword ptr [rdx]
	vmovaps ymmword ptr [r8], ymm0
	add rdx, rax
	add r8, rax
	loop @b
@@:
	ret
CopyY endp
; extern "C" void ZeroY(void* dst, size_t size)
ZeroY proc
	xchg rcx, rdx
	shr rcx, 5
	vxorps ymm0, ymm0, ymm0
	mov rax, 32
@@:
	vmovaps ymmword ptr [rdx], ymm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymmword ptr [rdx], ymm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymmword ptr [rdx], ymm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps ymmword ptr [rdx], ymm0
	add rdx, rax
	loop @b
@@:
	ret
ZeroY endp
end