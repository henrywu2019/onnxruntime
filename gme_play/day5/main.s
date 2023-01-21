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
.LFB8354:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movslq	%esi, %rbp
	movq	%rbp, %rax
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	shrq	$61, %rax
	jne	.L17
	pxor	%xmm0, %xmm0
	movq	$0, 16(%rdi)
	movq	%rdi, %r12
	leaq	0(,%rbp,4), %r14
	movups	%xmm0, (%rdi)
	testq	%rbp, %rbp
	je	.L3
	movq	%r14, %rdi
	call	_Znwm@PLT
	leaq	(%rax,%r14), %r13
	movq	%rax, (%r12)
	movq	%rax, %rbx
	leaq	4(%rax), %rdi
	movq	%r13, 16(%r12)
	movl	$0x00000000, (%rax)
	cmpq	$1, %rbp
	je	.L12
	cmpq	%r13, %rdi
	je	.L4
	leaq	-4(%r14), %rdx
	xorl	%esi, %esi
	call	memset@PLT
	movq	%rax, %rdi
.L4:
	movq	%r13, 8(%r12)
	cmpq	%rbx, %r13
	je	.L1
	movq	%r13, %rdx
	movq	%rbx, %rax
	subq	%rbx, %rdx
	subq	$4, %rdx
	movq	%rdx, %rcx
	shrq	$2, %rcx
	addq	$1, %rcx
	cmpq	$8, %rdx
	jbe	.L13
	movq	%rcx, %rdx
	movdqa	.LC0(%rip), %xmm1
	movdqa	.LC3(%rip), %xmm2
	shrq	$2, %rdx
	salq	$4, %rdx
	addq	%rbx, %rdx
	.p2align 4,,10
	.p2align 3
.L8:
	movdqa	%xmm1, %xmm0
	addq	$16, %rax
	paddd	%xmm2, %xmm1
	cvtdq2ps	%xmm0, %xmm0
	movups	%xmm0, -16(%rax)
	cmpq	%rax, %rdx
	jne	.L8
	movq	%rcx, %rdx
	andq	$-4, %rdx
	leaq	(%rbx,%rdx,4), %rbx
	leal	1(%rdx), %eax
	leaq	4(%rbx), %rdi
	cmpq	%rdx, %rcx
	je	.L1
.L7:
	pxor	%xmm0, %xmm0
	leal	1(%rax), %edx
	cvtsi2ssl	%eax, %xmm0
	movss	%xmm0, (%rbx)
	cmpq	%rdi, %r13
	je	.L1
	pxor	%xmm0, %xmm0
	addl	$2, %eax
	cvtsi2ssl	%edx, %xmm0
	leaq	8(%rbx), %rdx
	movss	%xmm0, 4(%rbx)
	cmpq	%rdx, %r13
	je	.L1
	pxor	%xmm0, %xmm0
	cvtsi2ssl	%eax, %xmm0
	movss	%xmm0, 8(%rbx)
.L1:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%r12, %rax
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	.cfi_restore_state
	movq	$0, 16(%rdi)
	movq	%r12, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L12:
	.cfi_restore_state
	movq	%rdi, %r13
	jmp	.L4
.L13:
	movl	$1, %eax
	jmp	.L7
.L17:
	leaq	.LC1(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE8354:
	.size	_Z9get_inputi, .-_Z9get_inputi
	.section	.rodata._ZNSt6vectorIfSaIfEE17_M_default_appendEm.str1.1,"aMS",@progbits,1
.LC4:
	.string	"vector::_M_default_append"
	.section	.text._ZNSt6vectorIfSaIfEE17_M_default_appendEm,"axG",@progbits,_ZNSt6vectorIfSaIfEE17_M_default_appendEm,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	.type	_ZNSt6vectorIfSaIfEE17_M_default_appendEm, @function
_ZNSt6vectorIfSaIfEE17_M_default_appendEm:
.LFB9118:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	je	.L47
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
	movq	(%rdi), %r8
	movq	%rdx, %rbp
	subq	%r8, %rbp
	movq	%rbp, %r13
	sarq	$2, %r13
	subq	%r13, %rax
	movq	%rax, %rcx
	movq	16(%rdi), %rax
	subq	%rdx, %rax
	sarq	$2, %rax
	cmpq	%rax, %rsi
	jbe	.L50
	cmpq	%rsi, %rcx
	jb	.L51
	cmpq	%r13, %rsi
	movq	%r13, %rax
	cmovnb	%rsi, %rax
	addq	%r13, %rax
	jc	.L24
	testq	%rax, %rax
	jne	.L52
	movq	%rbp, %r9
	xorl	%r14d, %r14d
	xorl	%r15d, %r15d
.L26:
	addq	%r15, %rbp
	movq	%rbx, %rdx
	movl	$0x00000000, 0(%rbp)
	subq	$1, %rdx
	je	.L30
	leaq	4(%rbp), %rdi
	salq	$2, %rdx
	xorl	%esi, %esi
	movq	%r9, 8(%rsp)
	movq	%r8, (%rsp)
	call	memset@PLT
	movq	(%rsp), %r8
	movq	8(%rsp), %r9
.L30:
	testq	%r9, %r9
	jg	.L53
	testq	%r8, %r8
	jne	.L31
.L32:
	addq	%r13, %rbx
	movq	%r15, %xmm0
	movq	%r14, 16(%r12)
	leaq	(%r15,%rbx,4), %rax
	movq	%rax, %xmm1
	punpcklqdq	%xmm1, %xmm0
	movups	%xmm0, (%r12)
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
.L50:
	.cfi_restore_state
	movl	$0x00000000, (%rdx)
	leaq	4(%rdx), %rcx
	subq	$1, %rbx
	je	.L21
	leaq	(%rcx,%rbx,4), %rax
	movq	%rcx, %rdi
	xorl	%esi, %esi
	subq	%rdx, %rax
	leaq	-4(%rax), %rbx
	movq	%rbx, %rdx
	call	memset@PLT
	movq	%rax, %rcx
	addq	%rbx, %rcx
.L21:
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
.L47:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.p2align 4,,10
	.p2align 3
.L53:
	.cfi_def_cfa_offset 80
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%r8, %rsi
	movq	%r9, %rdx
	movq	%r15, %rdi
	movq	%r8, (%rsp)
	call	memmove@PLT
	movq	(%rsp), %r8
.L31:
	movq	%r8, %rdi
	call	_ZdlPv@PLT
	jmp	.L32
.L52:
	movabsq	$2305843009213693951, %rdx
	cmpq	%rdx, %rax
	cmova	%rdx, %rax
	leaq	0(,%rax,4), %r14
.L25:
	movq	%r14, %rdi
	call	_Znwm@PLT
	movq	(%r12), %r8
	movq	8(%r12), %r9
	movq	%rax, %r15
	addq	%rax, %r14
	subq	%r8, %r9
	jmp	.L26
.L24:
	movabsq	$9223372036854775804, %r14
	jmp	.L25
.L51:
	leaq	.LC4(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE9118:
	.size	_ZNSt6vectorIfSaIfEE17_M_default_appendEm, .-_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	.text
	.p2align 4
	.globl	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	.type	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE, @function
_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE:
.LFB8365:
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
	ja	.L62
.L55:
	movl	4(%rsp), %eax
	testl	%eax, %eax
	jle	.L54
	salq	$2, %r13
	movslq	%r8d, %rbp
	salq	$2, %rbx
	xorl	%r15d, %r15d
	leaq	0(,%rbp,4), %rax
	xorl	%ebp, %ebp
	movq	%rax, 8(%rsp)
	.p2align 4,,10
	.p2align 3
.L58:
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
	jne	.L58
.L54:
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
.L62:
	.cfi_restore_state
	movl	4(%rsp), %esi
	imull	%r13d, %esi
	movslq	%esi, %rsi
	cmpq	%rax, %rsi
	ja	.L63
	jnb	.L55
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rax, %rdx
	je	.L55
	movq	%rax, 8(%r12)
	jmp	.L55
	.p2align 4,,10
	.p2align 3
.L63:
	subq	%rax, %rsi
	movq	%r12, %rdi
	movl	%r8d, 8(%rsp)
	call	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	movl	8(%rsp), %r8d
	jmp	.L55
	.cfi_endproc
.LFE8365:
	.size	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE, .-_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	.section	.text.unlikely,"ax",@progbits
.LCOLDB5:
	.section	.text.startup,"ax",@progbits
.LHOTB5:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB8366:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA8366
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$36, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$168, %rsp
	.cfi_def_cfa_offset 192
	movq	%fs:40, %rax
	movq	%rax, 152(%rsp)
	xorl	%eax, %eax
	leaq	64(%rsp), %rdi
.LEHB0:
	call	_Z9get_inputi
.LEHE0:
	leaq	96(%rsp), %rdi
	movl	$9, %esi
.LEHB1:
	call	_Z9get_inputi
.LEHE1:
	pxor	%xmm0, %xmm0
	movl	$96, %edi
	movups	%xmm0, 136(%rsp)
.LEHB2:
	call	_Znwm@PLT
.LEHE2:
	leaq	8(%rax), %rdi
	movq	%rax, %rcx
	movq	%rax, %xmm0
	movq	%rax, %r8
	andq	$-8, %rdi
	leaq	96(%rax), %rdx
	punpcklqdq	%xmm0, %xmm0
	xorl	%ebx, %ebx
	subq	%rdi, %rcx
	movq	%rdx, 144(%rsp)
	movl	$24, %ebp
	addl	$96, %ecx
	movaps	%xmm0, 128(%rsp)
	shrl	$3, %ecx
	movq	$0, (%rax)
	movq	$0, 88(%rax)
	xorl	%eax, %eax
	rep stosq
	movq	%rdx, 136(%rsp)
	subq	%r8, %rdx
	cmpq	$60, %rdx
	jbe	.L65
.L68:
	movq	64(%rsp), %rax
	movdqu	(%rax,%rbx), %xmm0
	movups	%xmm0, (%r8)
	movq	64(%rsp), %rax
	movdqu	24(%rax,%rbx), %xmm2
	movups	%xmm2, 16(%r8)
	movdqu	48(%rax,%rbx), %xmm1
	movups	%xmm1, 32(%r8)
	movdqu	72(%rax,%rbx), %xmm4
	movups	%xmm4, 48(%r8)
	movss	56+O(%rip), %xmm5
	movss	48+O(%rip), %xmm3
	movss	52+O(%rip), %xmm15
	movaps	%xmm4, 16(%rsp)
	movdqu	96(%rax,%rbx), %xmm4
	movss	%xmm5, 48(%rsp)
	movaps	16(%rsp), %xmm5
	movups	%xmm4, 64(%r8)
	movaps	%xmm4, (%rsp)
	movdqu	120(%rax,%rbx), %xmm4
	movq	96(%rsp), %rax
	leaq	(%rax,%rbx), %rdx
	movups	%xmm4, 80(%r8)
	movups	(%rdx), %xmm6
	movss	(%rdx), %xmm14
	movaps	%xmm4, 32(%rsp)
	shufps	$0, %xmm6, %xmm6
	mulps	%xmm6, %xmm5
	mulps	%xmm6, %xmm1
	addps	48+O(%rip), %xmm5
	mulps	%xmm6, %xmm2
	addps	32+O(%rip), %xmm1
	mulps	%xmm0, %xmm6
	addps	16+O(%rip), %xmm2
	addps	O(%rip), %xmm6
	movaps	%xmm1, 32+O(%rip)
	movaps	%xmm2, 16+O(%rip)
	movaps	%xmm6, O(%rip)
	movaps	%xmm5, 48+O(%rip)
	movups	(%rdx), %xmm0
	movl	(%rsp), %edx
	movaps	(%rsp), %xmm7
	movss	20(%r8), %xmm12
	shufps	$255, %xmm0, %xmm0
	movss	24(%r8), %xmm11
	movss	16(%r8), %xmm13
	mulps	%xmm0, %xmm7
	movss	28(%r8), %xmm10
	mulss	%xmm0, %xmm12
	mulss	%xmm0, %xmm11
	mulss	%xmm0, %xmm13
	mulss	%xmm0, %xmm10
	movaps	%xmm7, %xmm4
	movaps	%xmm6, %xmm7
	shufps	$85, %xmm6, %xmm7
	addss	%xmm7, %xmm12
	movaps	%xmm6, %xmm7
	addps	%xmm5, %xmm4
	unpckhps	%xmm6, %xmm7
	movss	12(%rax,%rbx), %xmm5
	addss	%xmm7, %xmm11
	movss	32(%r8), %xmm7
	addss	%xmm6, %xmm13
	shufps	$255, %xmm6, %xmm6
	movss	%xmm12, 4+O(%rip)
	addss	%xmm6, %xmm10
	movss	36(%r8), %xmm6
	mulss	%xmm0, %xmm7
	movss	%xmm13, O(%rip)
	mulss	%xmm0, %xmm6
	movss	%xmm11, 8+O(%rip)
	movss	%xmm10, 12+O(%rip)
	addss	%xmm2, %xmm7
	movss	%xmm7, 52(%rsp)
	movss	%xmm7, 16+O(%rip)
	movaps	%xmm6, %xmm7
	movaps	%xmm2, %xmm6
	movaps	%xmm7, %xmm8
	shufps	$85, %xmm2, %xmm6
	addss	%xmm6, %xmm8
	movss	40(%r8), %xmm6
	mulss	%xmm0, %xmm6
	movss	%xmm8, 56(%rsp)
	movss	%xmm8, 20+O(%rip)
	movaps	%xmm6, %xmm7
	movaps	%xmm2, %xmm6
	unpckhps	%xmm2, %xmm6
	movaps	%xmm7, %xmm9
	shufps	$255, %xmm2, %xmm2
	addss	%xmm6, %xmm9
	movss	44(%r8), %xmm6
	mulss	%xmm0, %xmm6
	movss	%xmm9, 60(%rsp)
	movss	%xmm9, 24+O(%rip)
	movaps	%xmm6, %xmm9
	movss	56(%r8), %xmm6
	addss	%xmm2, %xmm9
	movss	48(%r8), %xmm2
	mulss	%xmm0, %xmm6
	mulss	%xmm0, %xmm2
	movss	%xmm9, 28+O(%rip)
	movaps	%xmm2, %xmm8
	movss	52(%r8), %xmm2
	addss	%xmm1, %xmm8
	mulss	%xmm0, %xmm2
	mulss	60(%r8), %xmm0
	movss	%xmm8, 32+O(%rip)
	movaps	%xmm2, %xmm7
	movaps	%xmm1, %xmm2
	shufps	$85, %xmm1, %xmm2
	addss	%xmm2, %xmm7
	movaps	%xmm1, %xmm2
	unpckhps	%xmm1, %xmm2
	shufps	$255, %xmm1, %xmm1
	addss	%xmm1, %xmm0
	movd	16(%rsp), %xmm1
	addss	%xmm2, %xmm6
	movd	%edx, %xmm2
	mulss	%xmm14, %xmm1
	movss	%xmm7, 36+O(%rip)
	movss	%xmm0, 44+O(%rip)
	movss	%xmm6, 40+O(%rip)
	addss	%xmm3, %xmm1
	movaps	%xmm5, %xmm3
	mulss	%xmm2, %xmm3
	addss	%xmm3, %xmm1
	movss	%xmm1, 48+O(%rip)
	movq	(%rsp), %rsi
	movl	8(%rsp), %edx
	movq	16(%rsp), %rcx
	shrq	$32, %rsi
	shrq	$32, %rcx
	movd	%esi, %xmm1
	movq	%rcx, %xmm3
	mulss	%xmm14, %xmm3
	addss	%xmm15, %xmm3
	movaps	%xmm5, %xmm15
	mulss	%xmm1, %xmm15
	addss	%xmm15, %xmm3
	movss	%xmm3, 52+O(%rip)
	movd	%edx, %xmm3
	movl	24(%rsp), %edx
	mulss	%xmm3, %xmm5
	movd	%edx, %xmm15
	mulss	%xmm15, %xmm14
	addss	48(%rsp), %xmm14
	addss	%xmm5, %xmm14
	movaps	%xmm4, %xmm5
	shufps	$255, %xmm4, %xmm5
	movss	%xmm5, 60+O(%rip)
	movss	%xmm14, 56+O(%rip)
	movss	24(%rax,%rbx), %xmm5
	movss	32(%r8), %xmm14
	addq	$4, %rbx
	movq	8(%rsp), %rax
	mulss	%xmm5, %xmm14
	mulss	%xmm5, %xmm1
	shrq	$32, %rax
	mulss	%xmm5, %xmm2
	mulss	%xmm5, %xmm3
	addss	%xmm13, %xmm14
	movss	36(%r8), %xmm13
	addss	%xmm7, %xmm1
	mulss	%xmm5, %xmm13
	addss	%xmm8, %xmm2
	movss	%xmm14, O(%rip)
	addss	%xmm6, %xmm3
	movss	%xmm1, 36+O(%rip)
	movq	%rax, %xmm1
	mulss	%xmm5, %xmm1
	movss	%xmm2, 32+O(%rip)
	addss	%xmm12, %xmm13
	movss	40(%r8), %xmm12
	movss	%xmm3, 40+O(%rip)
	mulss	%xmm5, %xmm12
	movss	%xmm13, 4+O(%rip)
	addss	%xmm0, %xmm1
	addss	%xmm11, %xmm12
	movss	44(%r8), %xmm11
	mulss	%xmm5, %xmm11
	movss	%xmm12, 8+O(%rip)
	addss	%xmm10, %xmm11
	movss	48(%r8), %xmm10
	mulss	%xmm5, %xmm10
	movss	%xmm11, 12+O(%rip)
	addss	52(%rsp), %xmm10
	movss	%xmm10, 16+O(%rip)
	movss	52(%r8), %xmm10
	mulss	%xmm5, %xmm10
	addss	56(%rsp), %xmm10
	movss	%xmm10, 20+O(%rip)
	movss	56(%r8), %xmm10
	mulss	%xmm5, %xmm10
	addss	60(%rsp), %xmm10
	movss	%xmm10, 24+O(%rip)
	movss	60(%r8), %xmm10
	mulss	%xmm5, %xmm10
	shufps	$0, %xmm5, %xmm5
	addss	%xmm9, %xmm10
	movss	%xmm10, 28+O(%rip)
	movss	%xmm1, 44+O(%rip)
	mulps	32(%rsp), %xmm5
	addps	%xmm4, %xmm5
	movaps	%xmm5, 48+O(%rip)
	cmpq	$12, %rbx
	je	.L100
	movq	136(%rsp), %rdx
	subq	%r8, %rdx
	cmpq	$60, %rdx
	ja	.L68
.L65:
	sarq	$2, %rdx
	movq	%rbp, %rsi
	leaq	128(%rsp), %rdi
	subq	%rdx, %rsi
.LEHB3:
	call	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
.LEHE3:
	movq	128(%rsp), %r8
	jmp	.L68
.L100:
	movq	%r8, %rdi
	call	_ZdlPv@PLT
	movq	96(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L70
	call	_ZdlPv@PLT
.L70:
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L71
	call	_ZdlPv@PLT
.L71:
	movq	152(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L101
	addq	$168, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L101:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
.L80:
	endbr64
	movq	%rax, %rbp
	jmp	.L74
.L79:
	endbr64
	movq	%rax, %rbp
	jmp	.L76
.L81:
	endbr64
	movq	%rax, %rbp
	jmp	.L72
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA8366:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE8366-.LLSDACSB8366
.LLSDACSB8366:
	.uleb128 .LEHB0-.LFB8366
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB8366
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L79-.LFB8366
	.uleb128 0
	.uleb128 .LEHB2-.LFB8366
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L80-.LFB8366
	.uleb128 0
	.uleb128 .LEHB3-.LFB8366
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L81-.LFB8366
	.uleb128 0
.LLSDACSE8366:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC8366
	.type	main.cold, @function
main.cold:
.LFSB8366:
.L72:
	.cfi_def_cfa_offset 192
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movq	128(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L74
	call	_ZdlPv@PLT
.L74:
	movq	96(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L76
	call	_ZdlPv@PLT
.L76:
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L77
	call	_ZdlPv@PLT
.L77:
	movq	%rbp, %rdi
.LEHB4:
	call	_Unwind_Resume@PLT
.LEHE4:
	.cfi_endproc
.LFE8366:
	.section	.gcc_except_table
.LLSDAC8366:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC8366-.LLSDACSBC8366
.LLSDACSBC8366:
	.uleb128 .LEHB4-.LCOLDB5
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSEC8366:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE5:
	.section	.text.startup
.LHOTE5:
	.p2align 4
	.type	_GLOBAL__sub_I__Z9get_inputi, @function
_GLOBAL__sub_I__Z9get_inputi:
.LFB9620:
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
.LFE9620:
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
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	1
	.long	2
	.long	3
	.long	4
	.align 16
.LC3:
	.long	4
	.long	4
	.long	4
	.long	4
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
