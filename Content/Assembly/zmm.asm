.code
SwapZ proc
	vmovaps zmm0, zmmword ptr [rcx]
	vmovaps zmm1, zmmword ptr [rdx]
	vmovaps zmmword ptr [rdx], zmm0
	vmovaps zmmword ptr [rcx], zmm1
	ret
SwapZ endp

; extern "C" void CopyZ(void* dst, void* src, size_t r_size)
CopyZ proc
	xchg rcx, r8
	shr rcx, 6
	mov rax, 64
@@:
	vmovaps zmm0, zmmword ptr [rdx]
	vmovaps zmmword ptr [r8], zmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmm0, zmmword ptr [rdx]
	vmovaps zmmword ptr [r8], zmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmm0, zmmword ptr [rdx]
	vmovaps zmmword ptr [r8], zmm0
	add rdx, rax
	add r8, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmm0, zmmword ptr [rdx]
	vmovaps zmmword ptr [r8], zmm0
	add rdx, rax
	add r8, rax
	loop @b
@@:
	ret
CopyZ endp
; extern "C" void ZeroY(void* dst, size_t size)
ZeroZ proc
	xchg rcx, rdx
	shr rcx, 6
	vxorps zmm0, zmm0, zmm0
	mov rax, 64
@@:
	vmovaps zmmword ptr [rdx], zmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmmword ptr [rdx], zmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmmword ptr [rdx], zmm0
	add rdx, rax
	dec rcx
	test rcx, rcx
	jz @f
	vmovaps zmmword ptr [rdx], zmm0
	add rdx, rax
	loop @b
@@:
	ret
ZeroZ endp
end