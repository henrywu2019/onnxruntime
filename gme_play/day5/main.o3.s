	.file	"main.cpp"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"cannot create std::vector larger than max_size()"
	.text
	.p2align 4
	.globl	_Z9get_inputi
	.type	_Z9get_inputi, @function
_Z9get_inputi:
.LFB8889:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	movslq	%esi, %r13
	pushq	%r12
	movq	%r13, %rax
	pushq	%rbx
	andq	$-32, %rsp
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	shrq	$61, %rax
	jne	.L20
	vpxor	%xmm0, %xmm0, %xmm0
	movq	$0, 16(%rdi)
	movq	%rdi, %r12
	leaq	0(,%r13,4), %r15
	vmovdqu	%xmm0, (%rdi)
	testq	%r13, %r13
	je	.L3
	movq	%r15, %rdi
	call	_Znwm@PLT
	leaq	(%rax,%r15), %r14
	movq	%rax, (%r12)
	movq	%rax, %rbx
	leaq	4(%rax), %rdi
	movq	%r14, 16(%r12)
	movl	$0x00000000, (%rax)
	cmpq	$1, %r13
	je	.L14
	cmpq	%r14, %rdi
	je	.L4
	leaq	-4(%r15), %rdx
	xorl	%esi, %esi
	call	memset@PLT
.L4:
	movq	%r14, 8(%r12)
	cmpq	%rbx, %r14
	je	.L1
	movq	%r14, %rdx
	movq	%rbx, %rax
	subq	%rbx, %rdx
	subq	$4, %rdx
	movq	%rdx, %rsi
	shrq	$2, %rsi
	leaq	1(%rsi), %rdi
	cmpq	$24, %rdx
	jbe	.L15
	movq	%rdi, %rdx
	vmovdqa	.LC0(%rip), %ymm1
	vmovdqa	.LC3(%rip), %ymm2
	shrq	$3, %rdx
	salq	$5, %rdx
	addq	%rbx, %rdx
	.p2align 4,,10
	.p2align 3
.L8:
	vmovdqa	%ymm1, %ymm0
	addq	$32, %rax
	vpaddd	%ymm2, %ymm1, %ymm1
	vcvtdq2ps	%ymm0, %ymm0
	vmovups	%ymm0, -32(%rax)
	cmpq	%rax, %rdx
	jne	.L8
	movq	%rdi, %rcx
	andq	$-8, %rcx
	leal	1(%rcx), %eax
	leaq	(%rbx,%rcx,4), %rdx
	cmpq	%rcx, %rdi
	je	.L21
	vzeroupper
.L7:
	subq	%rcx, %rsi
	leaq	1(%rsi), %rdi
	cmpq	$2, %rsi
	jbe	.L12
	vmovd	%eax, %xmm3
	vpshufd	$0, %xmm3, %xmm0
	vpaddd	.LC4(%rip), %xmm0, %xmm0
	vcvtdq2ps	%xmm0, %xmm0
	vmovups	%xmm0, (%rbx,%rcx,4)
	movq	%rdi, %rcx
	andq	$-4, %rcx
	addl	%ecx, %eax
	leaq	(%rdx,%rcx,4), %rdx
	cmpq	%rcx, %rdi
	je	.L1
.L12:
	vxorps	%xmm0, %xmm0, %xmm0
	leaq	4(%rdx), %rcx
	leal	1(%rax), %esi
	vcvtsi2ssl	%eax, %xmm0, %xmm1
	vmovss	%xmm1, (%rdx)
	cmpq	%rcx, %r14
	je	.L1
	vcvtsi2ssl	%esi, %xmm0, %xmm1
	leaq	8(%rdx), %rcx
	addl	$2, %eax
	vmovss	%xmm1, 4(%rdx)
	cmpq	%rcx, %r14
	je	.L1
	vcvtsi2ssl	%eax, %xmm0, %xmm0
	vmovss	%xmm0, 8(%rdx)
.L1:
	leaq	-40(%rbp), %rsp
	movq	%r12, %rax
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	.cfi_restore_state
	movq	$0, 16(%rdi)
	jmp	.L1
	.p2align 4,,10
	.p2align 3
.L14:
	movq	%rdi, %r14
	jmp	.L4
	.p2align 4,,10
	.p2align 3
.L21:
	vzeroupper
	jmp	.L1
.L15:
	movq	%rbx, %rdx
	xorl	%ecx, %ecx
	movl	$1, %eax
	jmp	.L7
.L20:
	leaq	.LC1(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE8889:
	.size	_Z9get_inputi, .-_Z9get_inputi
	.section	.rodata._ZNSt6vectorIfSaIfEE17_M_default_appendEm.str1.1,"aMS",@progbits,1
.LC5:
	.string	"vector::_M_default_append"
	.section	.text._ZNSt6vectorIfSaIfEE17_M_default_appendEm,"axG",@progbits,_ZNSt6vectorIfSaIfEE17_M_default_appendEm,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	.type	_ZNSt6vectorIfSaIfEE17_M_default_appendEm, @function
_ZNSt6vectorIfSaIfEE17_M_default_appendEm:
.LFB9661:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	je	.L52
	movabsq	$2305843009213693951, %rax
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	8(%rdi), %rdx
	movq	(%rdi), %r14
	movq	%rdx, %rbp
	subq	%r14, %rbp
	movq	%rbp, %r13
	sarq	$2, %r13
	subq	%r13, %rax
	movq	%rax, %rcx
	movq	16(%rdi), %rax
	subq	%rdx, %rax
	sarq	$2, %rax
	cmpq	%rax, %rsi
	jbe	.L55
	cmpq	%rsi, %rcx
	jb	.L56
	cmpq	%r13, %rsi
	movq	%r13, %rax
	cmovnb	%rsi, %rax
	addq	%r13, %rax
	jc	.L28
	testq	%rax, %rax
	jne	.L57
	movq	%rbp, %r8
	xorl	%r15d, %r15d
	xorl	%ecx, %ecx
.L30:
	addq	%rcx, %rbp
	movq	%rbx, %rdx
	movl	$0x00000000, 0(%rbp)
	subq	$1, %rdx
	je	.L34
	leaq	4(%rbp), %rdi
	salq	$2, %rdx
	xorl	%esi, %esi
	movq	%r8, 8(%rsp)
	movq	%rcx, (%rsp)
	call	memset@PLT
	movq	(%rsp), %rcx
	movq	8(%rsp), %r8
.L34:
	testq	%r8, %r8
	jg	.L58
	testq	%r14, %r14
	jne	.L35
.L36:
	addq	%r13, %rbx
	vmovq	%rcx, %xmm1
	movq	%r15, 16(%r12)
	leaq	(%rcx,%rbx,4), %rax
	vpinsrq	$1, %rax, %xmm1, %xmm0
	vmovdqu	%xmm0, (%r12)
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L55:
	.cfi_restore_state
	movl	$0x00000000, (%rdx)
	leaq	4(%rdx), %rcx
	subq	$1, %rbx
	je	.L25
	leaq	(%rcx,%rbx,4), %rax
	movq	%rcx, %rdi
	xorl	%esi, %esi
	subq	%rdx, %rax
	leaq	-4(%rax), %rbx
	movq	%rbx, %rdx
	call	memset@PLT
	movq	%rax, %rcx
	addq	%rbx, %rcx
.L25:
	movq	%rcx, 8(%r12)
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L52:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.p2align 4,,10
	.p2align 3
.L58:
	.cfi_def_cfa_offset 80
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rcx, %rdi
	movq	%r8, %rdx
	movq	%r14, %rsi
	call	memmove@PLT
	movq	%rax, %rcx
.L35:
	movq	%r14, %rdi
	movq	%rcx, (%rsp)
	call	_ZdlPv@PLT
	movq	(%rsp), %rcx
	jmp	.L36
.L57:
	movabsq	$2305843009213693951, %rdx
	cmpq	%rdx, %rax
	cmova	%rdx, %rax
	leaq	0(,%rax,4), %r15
.L29:
	movq	%r15, %rdi
	call	_Znwm@PLT
	movq	(%r12), %r14
	movq	8(%r12), %r8
	movq	%rax, %rcx
	addq	%rax, %r15
	subq	%r14, %r8
	jmp	.L30
.L28:
	movabsq	$9223372036854775804, %r15
	jmp	.L29
.L56:
	leaq	.LC5(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE9661:
	.size	_ZNSt6vectorIfSaIfEE17_M_default_appendEm, .-_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC6:
	.string	" | Compute Time: "
.LC7:
	.string	" ns"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB8:
	.section	.text.startup,"ax",@progbits
.LHOTB8:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB8901:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA8901
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$36, %esi
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	addq	$-128, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	leaq	32(%rsp), %rdi
.LEHB0:
	call	_Z9get_inputi
.LEHE0:
	leaq	64(%rsp), %rdi
	movl	$9, %esi
.LEHB1:
	call	_Z9get_inputi
.LEHE1:
	vpxor	%xmm0, %xmm0, %xmm0
	movl	$96, %edi
	vmovdqu	%xmm0, 104(%rsp)
.LEHB2:
	call	_Znwm@PLT
.LEHE2:
	leaq	8(%rax), %rdi
	movq	%rax, %rcx
	vmovq	%rax, %xmm6
	movq	%rax, %r12
	andq	$-8, %rdi
	leaq	96(%rax), %r13
	vpunpcklqdq	%xmm6, %xmm6, %xmm0
	xorl	%ebx, %ebx
	subq	%rdi, %rcx
	movq	%r13, 112(%rsp)
	addl	$96, %ecx
	vmovdqa	%xmm0, 96(%rsp)
	shrl	$3, %ecx
	movq	$0, (%rax)
	movq	$0, 88(%rax)
	xorl	%eax, %eax
	rep stosq
	movq	%r13, 104(%rsp)
	subq	%r12, %r13
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movq	%rax, (%rsp)
	cmpq	$60, %r13
	jbe	.L60
.L63:
	movq	32(%rsp), %rax
	movq	(%rax,%rbx), %r9
	movq	8(%rax,%rbx), %r10
	movq	%r9, (%r12)
	movq	32(%rsp), %r15
	movq	%r10, 8(%r12)
	movq	24(%r15,%rbx), %r11
	movq	32(%r15,%rbx), %r13
	movq	%r11, 16(%r12)
	movq	%r13, 24(%r12)
	movq	48(%r15,%rbx), %rdx
	movq	56(%r15,%rbx), %rcx
	movq	%rdx, 32(%r12)
	vmovd	%edx, %xmm1
	shrq	$32, %rdx
	movq	%rcx, 40(%r12)
	movq	72(%r15,%rbx), %rsi
	vmovd	%ecx, %xmm8
	shrq	$32, %rcx
	movq	80(%r15,%rbx), %r14
	vmovq	%rdx, %xmm11
	vmovq	%rcx, %xmm12
	movq	%rsi, 48(%r12)
	vmovd	%esi, %xmm2
	shrq	$32, %rsi
	movq	%r14, 56(%r12)
	movq	96(%r15,%rbx), %rax
	vmovq	%rsi, %xmm13
	movq	104(%r15,%rbx), %r8
	movq	%rax, 8(%rsp)
	movq	%rax, 64(%r12)
	movq	64(%rsp), %rax
	movq	%r8, 72(%r12)
	movq	120(%r15,%rbx), %rdi
	movq	128(%r15,%rbx), %r15
	movq	%rdi, 80(%r12)
	movq	%r15, 88(%r12)
	vmovss	(%rax,%rbx), %xmm0
	vmulss	%xmm2, %xmm0, %xmm9
	vbroadcastss	%xmm0, %ymm7
	vunpcklps	%xmm13, %xmm2, %xmm2
	vmulss	%xmm1, %xmm0, %xmm6
	vunpcklps	%xmm11, %xmm1, %xmm1
	vmulss	%xmm11, %xmm0, %xmm5
	vaddss	32+O(%rip), %xmm6, %xmm6
	vmulss	%xmm8, %xmm0, %xmm4
	vaddss	36+O(%rip), %xmm5, %xmm5
	vunpcklps	%xmm12, %xmm8, %xmm8
	vmulss	%xmm12, %xmm0, %xmm3
	vaddss	40+O(%rip), %xmm4, %xmm4
	vmovlhps	%xmm8, %xmm1, %xmm1
	vaddss	44+O(%rip), %xmm3, %xmm3
	vaddss	48+O(%rip), %xmm9, %xmm14
	vmulss	%xmm13, %xmm0, %xmm9
	vaddss	52+O(%rip), %xmm9, %xmm10
	vmovd	%r14d, %xmm9
	shrq	$32, %r14
	vmovd	%r10d, %xmm8
	vmulss	%xmm9, %xmm0, %xmm0
	vmovd	%r14d, %xmm15
	shrq	$32, %r10
	vaddss	56+O(%rip), %xmm0, %xmm0
	vunpcklps	%xmm15, %xmm9, %xmm9
	vmovd	%r10d, %xmm13
	vmovss	%xmm14, 24(%rsp)
	vmovlhps	%xmm9, %xmm2, %xmm2
	vmovd	%r13d, %xmm9
	shrq	$32, %r13
	vunpcklps	%xmm13, %xmm8, %xmm8
	vinsertf128	$0x1, %xmm2, %ymm1, %ymm1
	vmovd	%r13d, %xmm11
	vmovd	%r9d, %xmm2
	shrq	$32, %r9
	vmovss	%xmm0, 20(%rsp)
	vmovd	%r11d, %xmm0
	shrq	$32, %r11
	vunpcklps	%xmm11, %xmm9, %xmm9
	vmovd	%r11d, %xmm12
	vmovd	%r9d, %xmm11
	vmovss	%xmm10, 28(%rsp)
	vmulps	%ymm7, %ymm1, %ymm1
	vunpcklps	%xmm12, %xmm0, %xmm0
	vunpcklps	%xmm11, %xmm2, %xmm2
	vmovlhps	%xmm9, %xmm0, %xmm0
	vmovlhps	%xmm8, %xmm2, %xmm2
	vaddps	32+O(%rip), %ymm1, %ymm1
	vinsertf128	$0x1, %xmm0, %ymm2, %ymm2
	vmulps	%ymm7, %ymm2, %ymm2
	vaddps	O(%rip), %ymm2, %ymm2
	vmovaps	%ymm1, 32+O(%rip)
	vmovaps	%ymm2, O(%rip)
	vmovss	12(%rax,%rbx), %xmm0
	vshufps	$85, %xmm2, %xmm2, %xmm8
	vmulss	20(%r12), %xmm0, %xmm13
	vmulss	24(%r12), %xmm0, %xmm12
	vshufps	$255, %xmm2, %xmm2, %xmm7
	vmulss	16(%r12), %xmm0, %xmm14
	vmulss	28(%r12), %xmm0, %xmm11
	vmulss	36(%r12), %xmm0, %xmm9
	vaddss	%xmm8, %xmm13, %xmm13
	vmulss	32(%r12), %xmm0, %xmm10
	vunpckhps	%xmm2, %xmm2, %xmm8
	vaddss	%xmm8, %xmm12, %xmm12
	vmulss	40(%r12), %xmm0, %xmm8
	vaddss	%xmm2, %xmm14, %xmm14
	vextractf128	$0x1, %ymm2, %xmm2
	vaddss	%xmm7, %xmm11, %xmm11
	vshufps	$85, %xmm2, %xmm2, %xmm7
	vmovss	%xmm13, 4+O(%rip)
	vaddss	%xmm7, %xmm9, %xmm9
	vunpckhps	%xmm2, %xmm2, %xmm7
	vmovss	%xmm12, 8+O(%rip)
	vaddss	%xmm2, %xmm10, %xmm10
	vshufps	$255, %xmm2, %xmm2, %xmm2
	vmovss	%xmm14, O(%rip)
	vaddss	%xmm7, %xmm8, %xmm8
	vmulss	44(%r12), %xmm0, %xmm7
	vmovss	%xmm11, 12+O(%rip)
	vmovss	%xmm9, 20+O(%rip)
	vmovss	%xmm10, 16+O(%rip)
	vmovss	%xmm8, 24+O(%rip)
	vaddss	%xmm2, %xmm7, %xmm7
	vmovss	48(%r12), %xmm2
	vmulss	%xmm2, %xmm0, %xmm15
	vmovss	%xmm7, 28+O(%rip)
	vaddss	%xmm6, %xmm15, %xmm15
	vmovss	52(%r12), %xmm6
	vunpcklps	%xmm6, %xmm2, %xmm2
	vmovss	%xmm15, 32+O(%rip)
	vmulss	%xmm6, %xmm0, %xmm15
	vaddss	%xmm5, %xmm15, %xmm15
	vmovss	%xmm15, 36+O(%rip)
	vmovss	56(%r12), %xmm5
	movq	8(%rsp), %rax
	vmulss	%xmm5, %xmm0, %xmm15
	vinsertps	$0x10, 60(%r12), %xmm5, %xmm5
	vmovlhps	%xmm5, %xmm2, %xmm2
	vaddss	%xmm4, %xmm15, %xmm15
	vmulss	60(%r12), %xmm0, %xmm4
	vmovss	%xmm15, 40+O(%rip)
	vmovd	%eax, %xmm15
	shrq	$32, %rax
	vaddss	%xmm3, %xmm4, %xmm4
	vmulss	%xmm15, %xmm0, %xmm3
	vaddss	24(%rsp), %xmm3, %xmm3
	vmovss	%xmm4, 44+O(%rip)
	vmovss	%xmm3, 48+O(%rip)
	vmovd	%eax, %xmm3
	vmulss	%xmm3, %xmm0, %xmm3
	vaddss	28(%rsp), %xmm3, %xmm3
	vmovss	%xmm3, 52+O(%rip)
	vmovd	%r8d, %xmm3
	shrq	$32, %r8
	vmulss	%xmm3, %xmm0, %xmm4
	vbroadcastss	%xmm0, %ymm0
	vaddss	20(%rsp), %xmm4, %xmm4
	vmovss	%xmm4, 56+O(%rip)
	vmovd	%r8d, %xmm4
	vunpcklps	%xmm4, %xmm3, %xmm4
	vmovd	%eax, %xmm3
	movq	64(%rsp), %rax
	vunpcklps	%xmm3, %xmm15, %xmm3
	vmovlhps	%xmm4, %xmm3, %xmm3
	vmovd	%r15d, %xmm4
	shrq	$32, %r15
	vinsertf128	$0x1, %xmm3, %ymm2, %ymm2
	vmovd	%r15d, %xmm6
	vmulps	%ymm2, %ymm0, %ymm0
	vunpcklps	%xmm6, %xmm4, %xmm4
	vaddps	%ymm1, %ymm0, %ymm1
	vextractf128	$0x1, %ymm1, %xmm0
	vextractps	$3, %xmm0, 60+O(%rip)
	vmovss	24(%rax,%rbx), %xmm0
	vmulss	32(%r12), %xmm0, %xmm2
	addq	$4, %rbx
	vaddss	%xmm14, %xmm2, %xmm2
	vmovss	%xmm2, O(%rip)
	vmulss	36(%r12), %xmm0, %xmm2
	vaddss	%xmm13, %xmm2, %xmm2
	vmovss	%xmm2, 4+O(%rip)
	vmulss	40(%r12), %xmm0, %xmm2
	vaddss	%xmm12, %xmm2, %xmm2
	vmovss	%xmm2, 8+O(%rip)
	vmulss	44(%r12), %xmm0, %xmm2
	vaddss	%xmm11, %xmm2, %xmm2
	vmovss	%xmm2, 12+O(%rip)
	vmulss	48(%r12), %xmm0, %xmm2
	vaddss	%xmm10, %xmm2, %xmm2
	vmovss	%xmm2, 16+O(%rip)
	vmulss	52(%r12), %xmm0, %xmm2
	vaddss	%xmm9, %xmm2, %xmm2
	vmovss	%xmm2, 20+O(%rip)
	vmulss	56(%r12), %xmm0, %xmm2
	vaddss	%xmm8, %xmm2, %xmm2
	vmovss	%xmm2, 24+O(%rip)
	vmulss	60(%r12), %xmm0, %xmm2
	vbroadcastss	%xmm0, %ymm0
	vaddss	%xmm7, %xmm2, %xmm2
	vmovss	%xmm2, 28+O(%rip)
	vmovd	%edi, %xmm2
	shrq	$32, %rdi
	vmovd	%edi, %xmm5
	vunpcklps	%xmm5, %xmm2, %xmm2
	vmovlhps	%xmm4, %xmm2, %xmm2
	vinsertf128	$0x1, %xmm2, %ymm3, %ymm3
	vmulps	%ymm3, %ymm0, %ymm0
	vaddps	%ymm1, %ymm0, %ymm0
	vmovaps	%ymm0, 32+O(%rip)
	cmpq	$12, %rbx
	je	.L99
	movq	104(%rsp), %r13
	subq	%r12, %r13
	cmpq	$60, %r13
	ja	.L63
.L60:
	sarq	$2, %r13
	movl	$24, %esi
	leaq	96(%rsp), %rdi
	subq	%r13, %rsi
	vzeroupper
.LEHB3:
	call	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	movq	96(%rsp), %r12
	jmp	.L63
.L99:
	vzeroupper
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movl	$53, %esi
	leaq	_ZSt4cout(%rip), %rdi
	movq	%rax, %rbx
	call	_ZNSolsEi@PLT
	movl	$17, %edx
	leaq	.LC6(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %r13
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	%rbx, %rsi
	movq	%r13, %rdi
	subq	(%rsp), %rsi
	call	_ZNSo9_M_insertIlEERSoT_@PLT
	movl	$3, %edx
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %r13
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	0(%r13), %rax
	movq	-24(%rax), %rax
	movq	240(%r13,%rax), %r14
	testq	%r14, %r14
	je	.L100
	cmpb	$0, 56(%r14)
	je	.L66
	movsbl	67(%r14), %esi
.L67:
	movq	%r13, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	movq	%r12, %rdi
	call	_ZdlPv@PLT
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L68
	call	_ZdlPv@PLT
.L68:
	movq	32(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L69
	call	_ZdlPv@PLT
.L69:
	movq	120(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L101
	leaq	-40(%rbp), %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L66:
	.cfi_restore_state
	movq	%r14, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	(%r14), %rax
	movl	$10, %esi
	movq	%r14, %rdi
	call	*48(%rax)
	movsbl	%al, %esi
	jmp	.L67
.L101:
	call	__stack_chk_fail@PLT
.L100:
	call	_ZSt16__throw_bad_castv@PLT
.LEHE3:
.L79:
	endbr64
	movq	%rax, %r12
	jmp	.L70
.L78:
	endbr64
	movq	%rax, %r12
	vzeroupper
	jmp	.L72
.L77:
	endbr64
	movq	%rax, %r12
	vzeroupper
	jmp	.L74
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA8901:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE8901-.LLSDACSB8901
.LLSDACSB8901:
	.uleb128 .LEHB0-.LFB8901
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB8901
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L77-.LFB8901
	.uleb128 0
	.uleb128 .LEHB2-.LFB8901
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L78-.LFB8901
	.uleb128 0
	.uleb128 .LEHB3-.LFB8901
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L79-.LFB8901
	.uleb128 0
.LLSDACSE8901:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC8901
	.type	main.cold, @function
main.cold:
.LFSB8901:
.L70:
	.cfi_def_cfa 6, 16
	.cfi_offset 3, -56
	.cfi_offset 6, -16
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
	movq	96(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L97
	vzeroupper
	call	_ZdlPv@PLT
.L72:
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L74
	call	_ZdlPv@PLT
.L74:
	movq	32(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L75
	call	_ZdlPv@PLT
.L75:
	movq	%r12, %rdi
.LEHB4:
	call	_Unwind_Resume@PLT
.LEHE4:
.L97:
	vzeroupper
	jmp	.L72
	.cfi_endproc
.LFE8901:
	.section	.gcc_except_table
.LLSDAC8901:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC8901-.LLSDACSBC8901
.LLSDACSBC8901:
	.uleb128 .LEHB4-.LCOLDB8
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSEC8901:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE8:
	.section	.text.startup
.LHOTE8:
	.text
	.p2align 4
	.globl	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	.type	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE, @function
_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE:
.LFB8900:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rdi, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%r9, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	sarq	$32, %r13
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	imull	%r13d, %r9d
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movslq	%edx, %rbx
	movslq	%r9d, %r9
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	8(%rsi), %rdx
	movl	%ecx, 4(%rsp)
	movq	(%rsi), %rcx
	movq	%rdx, %rax
	subq	%rcx, %rax
	sarq	$2, %rax
	cmpq	%rax, %r9
	ja	.L111
.L103:
	movl	4(%rsp), %eax
	testl	%eax, %eax
	jle	.L109
	salq	$2, %r13
	movslq	%r8d, %rbp
	salq	$2, %rbx
	xorl	%r15d, %r15d
	leaq	0(,%rbp,4), %rax
	xorl	%ebp, %ebp
	movq	%rax, 8(%rsp)
	.p2align 4,,10
	.p2align 3
.L106:
	movq	(%r12), %rdi
	movq	(%r14), %rsi
	movq	%r13, %rdx
	addl	$1, %r15d
	addq	%rbp, %rdi
	addq	%rbx, %rsi
	addq	%r13, %rbp
	call	memcpy@PLT
	addq	8(%rsp), %rbx
	cmpl	%r15d, 4(%rsp)
	jne	.L106
.L109:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L111:
	.cfi_restore_state
	movl	4(%rsp), %esi
	imull	%r13d, %esi
	movslq	%esi, %rsi
	cmpq	%rax, %rsi
	ja	.L112
	jnb	.L103
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rax, %rdx
	je	.L103
	movq	%rax, 8(%r12)
	jmp	.L103
	.p2align 4,,10
	.p2align 3
.L112:
	subq	%rax, %rsi
	movq	%r12, %rdi
	movl	%r8d, 8(%rsp)
	call	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	movl	8(%rsp), %r8d
	jmp	.L103
	.cfi_endproc
.LFE8900:
	.size	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE, .-_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	.section	.text.startup
	.p2align 4
	.type	_GLOBAL__sub_I__Z9get_inputi, @function
_GLOBAL__sub_I__Z9get_inputi:
.LFB10168:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	_ZStL8__ioinit(%rip), %rbp
	movq	%rbp, %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	movq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE10168:
	.size	_GLOBAL__sub_I__Z9get_inputi, .-_GLOBAL__sub_I__Z9get_inputi
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z9get_inputi
	.globl	O
	.bss
	.align 32
	.type	O, @object
	.size	O, 64
O:
	.zero	64
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.align 32
.LC3:
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC4:
	.long	0
	.long	1
	.long	2
	.long	3
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
