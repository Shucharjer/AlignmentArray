.code
; extern "C" void SwapX(void* ptr_1, void* ptr_2)
SwapX proc
	vmovaps xmm0, xmmword ptr [rcx]
	vmovaps xmm1, xmmword ptr [rdx]
	vmovaps xmmword ptr [rdx], xmm0
	vmovaps xmmword ptr [rcx], xmm1
	ret
SwapX endp


; extern "C" void CopyX(void* dst, void* src, size_t r_size)     
CopyX proc
	xchg rcx, r8
	shr rcx, 4
	mov rax, 16
@@:
	vmovaps xmm0, xmmword ptr [rdx]
	vmovaps xmmword ptr [r8], xmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmm0, xmmword ptr [rdx]
	vmovaps xmmword ptr [r8], xmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmm0, xmmword ptr [rdx]
	vmovaps xmmword ptr [r8], xmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmm0, xmmword ptr [rdx]
	vmovaps xmmword ptr [r8], xmm0
	add rdx, rax
	add r8, rax
	loop @b
@@:
	ret
CopyX endp
; extern "C" void ZeroX(void* dst, size_t size)
ZeroX proc
	xchg rcx, rdx
	shr rcx, 4
	vxorps xmm0, xmm0, xmm0
	mov rax, 16
@@:
	vmovaps xmmword ptr [rdx], xmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmmword ptr [rdx], xmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmmword ptr [rdx], xmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps xmmword ptr [rdx], xmm0
	add rdx, rax
	loop @b
@@:
	ret
ZeroX endp
end