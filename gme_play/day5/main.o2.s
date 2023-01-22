	.file	"main.cpp"
	.text
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1623:
	.cfi_startproc
	endbr64
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE1623:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"cannot create std::vector larger than max_size()"
	.text
	.p2align 4
	.globl	_Z9get_inputi
	.type	_Z9get_inputi, @function
_Z9get_inputi:
.LFB8889:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movslq	%esi, %r13
	movq	%r13, %rax
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	shrq	$61, %rax
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	jne	.L14
	movq	$0, (%rdi)
	movq	%rdi, %r12
	leaq	0(,%r13,4), %r14
	movq	$0, 8(%rdi)
	movq	$0, 16(%rdi)
	testq	%r13, %r13
	je	.L5
	movq	%r14, %rdi
	call	_Znwm@PLT
	leaq	(%rax,%r14), %rbp
	movq	%rax, (%r12)
	movq	%rax, %rbx
	leaq	4(%rax), %rdi
	movq	%rbp, 16(%r12)
	movl	$0x00000000, (%rax)
	cmpq	$1, %r13
	je	.L11
	cmpq	%rbp, %rdi
	je	.L6
	leaq	-4(%r14), %rdx
	xorl	%esi, %esi
	call	memset@PLT
.L6:
	movq	%rbp, 8(%r12)
	cmpq	%rbx, %rbp
	je	.L3
	vxorps	%xmm1, %xmm1, %xmm1
	movq	%rbx, %rax
	movl	$1, %edx
	.p2align 4,,10
	.p2align 3
.L9:
	vcvtsi2ssl	%edx, %xmm1, %xmm0
	addq	$4, %rax
	addl	$1, %edx
	vmovss	%xmm0, -4(%rax)
	cmpq	%rbp, %rax
	jne	.L9
.L3:
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
.L5:
	.cfi_restore_state
	movq	$0, (%rdi)
	movq	%r12, %rax
	movq	$0, 16(%rdi)
	movq	$0, 8(%rdi)
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
.L11:
	.cfi_restore_state
	movq	%rdi, %rbp
	jmp	.L6
.L14:
	leaq	.LC0(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE8889:
	.size	_Z9get_inputi, .-_Z9get_inputi
	.section	.rodata._ZNSt6vectorIfSaIfEE17_M_default_appendEm.str1.1,"aMS",@progbits,1
.LC2:
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
	je	.L45
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
	jbe	.L48
	cmpq	%rsi, %rcx
	jb	.L49
	cmpq	%r13, %rsi
	movq	%r13, %rax
	cmovnb	%rsi, %rax
	addq	%r13, %rax
	jc	.L21
	testq	%rax, %rax
	jne	.L50
	movq	%rbp, %r8
	xorl	%r15d, %r15d
	xorl	%ecx, %ecx
.L23:
	addq	%rcx, %rbp
	movq	%rbx, %rdx
	movl	$0x00000000, 0(%rbp)
	subq	$1, %rdx
	je	.L27
	leaq	4(%rbp), %rdi
	salq	$2, %rdx
	xorl	%esi, %esi
	movq	%r8, 8(%rsp)
	movq	%rcx, (%rsp)
	call	memset@PLT
	movq	(%rsp), %rcx
	movq	8(%rsp), %r8
.L27:
	testq	%r8, %r8
	jg	.L51
	testq	%r14, %r14
	jne	.L28
.L29:
	addq	%r13, %rbx
	movq	%rcx, (%r12)
	leaq	(%rcx,%rbx,4), %rax
	movq	%r15, 16(%r12)
	movq	%rax, 8(%r12)
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
.L48:
	.cfi_restore_state
	movl	$0x00000000, (%rdx)
	leaq	4(%rdx), %rcx
	subq	$1, %rbx
	je	.L18
	leaq	(%rcx,%rbx,4), %rax
	movq	%rcx, %rdi
	xorl	%esi, %esi
	subq	%rdx, %rax
	leaq	-4(%rax), %rbx
	movq	%rbx, %rdx
	call	memset@PLT
	movq	%rax, %rcx
	addq	%rbx, %rcx
.L18:
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
.L45:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.p2align 4,,10
	.p2align 3
.L51:
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
.L28:
	movq	%r14, %rdi
	movq	%rcx, (%rsp)
	call	_ZdlPv@PLT
	movq	(%rsp), %rcx
	jmp	.L29
.L50:
	movabsq	$2305843009213693951, %rdx
	cmpq	%rdx, %rax
	cmova	%rdx, %rax
	leaq	0(,%rax,4), %r15
.L22:
	movq	%r15, %rdi
	call	_Znwm@PLT
	movq	(%r12), %r14
	movq	8(%r12), %r8
	movq	%rax, %rcx
	addq	%rax, %r15
	subq	%r14, %r8
	jmp	.L23
.L21:
	movabsq	$9223372036854775804, %r15
	jmp	.L22
.L49:
	leaq	.LC2(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE9661:
	.size	_ZNSt6vectorIfSaIfEE17_M_default_appendEm, .-_ZNSt6vectorIfSaIfEE17_M_default_appendEm
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
	ja	.L61
.L53:
	movl	4(%rsp), %eax
	testl	%eax, %eax
	jle	.L59
	salq	$2, %r13
	movslq	%r8d, %rbp
	salq	$2, %rbx
	xorl	%r15d, %r15d
	leaq	0(,%rbp,4), %rax
	xorl	%ebp, %ebp
	movq	%rax, 8(%rsp)
	.p2align 4,,10
	.p2align 3
.L56:
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
	jne	.L56
.L59:
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
.L61:
	.cfi_restore_state
	movl	4(%rsp), %esi
	imull	%r13d, %esi
	movslq	%esi, %rsi
	cmpq	%rax, %rsi
	ja	.L62
	jnb	.L53
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rax, %rdx
	je	.L53
	movq	%rax, 8(%r12)
	jmp	.L53
	.p2align 4,,10
	.p2align 3
.L62:
	subq	%rax, %rsi
	movq	%r12, %rdi
	movl	%r8d, 8(%rsp)
	call	_ZNSt6vectorIfSaIfEE17_M_default_appendEm
	movl	8(%rsp), %r8d
	jmp	.L53
	.cfi_endproc
.LFE8900:
	.size	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE, .-_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	" | Compute Time: "
.LC4:
	.string	" ns"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB5:
	.section	.text.startup,"ax",@progbits
.LHOTB5:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB8901:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA8901
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$36, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$104, %rsp
	.cfi_def_cfa_offset 160
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %r13
	movq	%r13, %rdi
.LEHB0:
	call	_Z9get_inputi
.LEHE0:
	leaq	32(%rsp), %rdi
	movl	$9, %esi
.LEHB1:
	call	_Z9get_inputi
.LEHE1:
	movq	$0, 72(%rsp)
	movl	$96, %edi
	movq	$0, 80(%rsp)
.LEHB2:
	call	_Znwm@PLT
.LEHE2:
	leaq	8(%rax), %rdi
	leaq	96(%rax), %rdx
	movq	%rax, 64(%rsp)
	xorl	%ebp, %ebp
	andq	$-8, %rdi
	movq	%rdx, 80(%rsp)
	leaq	64(%rsp), %r15
	leaq	64+O(%rip), %rbx
	movq	$0, (%rax)
	movq	$0, 88(%rax)
	subq	%rdi, %rax
	leal	96(%rax), %ecx
	xorl	%eax, %eax
	shrl	$3, %ecx
	rep stosq
	movq	%rdx, 72(%rsp)
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movq	%rax, %r14
.L66:
	movl	$6, %r8d
	movl	$6, %ecx
	movl	%ebp, %edx
	movq	%r15, %rsi
	movabsq	$17179869188, %r9
	movq	%r13, %rdi
.LEHB3:
	call	_Z7extractRKSt6vectorIfSaIfEERS1_iiiSt4pairIiiE
	movq	32(%rsp), %rax
	movq	64(%rsp), %r12
	xorl	%ecx, %ecx
	leaq	(%rax,%rbp,4), %rsi
.L65:
	vmovss	(%rsi), %xmm1
	leaq	(%r12,%rcx,4), %rdx
	leaq	O(%rip), %rax
	.p2align 4,,10
	.p2align 3
.L64:
	vmulss	(%rdx), %xmm1, %xmm0
	addq	$4, %rax
	addq	$4, %rdx
	vaddss	-4(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rax)
	cmpq	%rax, %rbx
	jne	.L64
	addq	$4, %rcx
	addq	$12, %rsi
	cmpq	$12, %rcx
	jne	.L65
	addq	$1, %rbp
	cmpq	$3, %rbp
	jne	.L66
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movl	$53, %esi
	leaq	_ZSt4cout(%rip), %rdi
	movq	%rax, %rbx
	call	_ZNSolsEi@PLT
	movq	%rax, %rdi
	leaq	.LC3(%rip), %rsi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	movq	%rbx, %rsi
	movq	%rax, %rdi
	subq	%r14, %rsi
	call	_ZNSo9_M_insertIlEERSoT_@PLT
	movq	%rax, %rdi
	leaq	.LC4(%rip), %rsi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	movq	%rax, %rbp
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbp,%rax), %r13
	testq	%r13, %r13
	je	.L108
	cmpb	$0, 56(%r13)
	je	.L68
	movzbl	67(%r13), %eax
.L69:
	movsbl	%al, %esi
	movq	%rbp, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	testq	%r12, %r12
	je	.L70
	movq	%r12, %rdi
	call	_ZdlPv@PLT
.L70:
	movq	32(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L71
	call	_ZdlPv@PLT
.L71:
	movq	(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L72
	call	_ZdlPv@PLT
.L72:
	movq	88(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L109
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
.L68:
	.cfi_restore_state
	movq	%r13, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%r13), %rax
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx
	movq	48(%rax), %rdx
	movl	$10, %eax
	cmpq	%rcx, %rdx
	je	.L69
	movl	$10, %esi
	movq	%r13, %rdi
	call	*%rdx
	jmp	.L69
.L109:
	call	__stack_chk_fail@PLT
.L108:
	call	_ZSt16__throw_bad_castv@PLT
.LEHE3:
.L81:
	endbr64
	movq	%rax, %rbp
	vzeroupper
	jmp	.L77
.L83:
	endbr64
	movq	%rax, %rbp
	jmp	.L73
.L82:
	endbr64
	movq	%rax, %rbp
	vzeroupper
	jmp	.L75
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
	.uleb128 .L81-.LFB8901
	.uleb128 0
	.uleb128 .LEHB2-.LFB8901
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L82-.LFB8901
	.uleb128 0
	.uleb128 .LEHB3-.LFB8901
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L83-.LFB8901
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
.L73:
	.cfi_def_cfa_offset 160
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L106
	vzeroupper
	call	_ZdlPv@PLT
.L75:
	movq	32(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L77
	call	_ZdlPv@PLT
.L77:
	movq	(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L78
	call	_ZdlPv@PLT
.L78:
	movq	%rbp, %rdi
.LEHB4:
	call	_Unwind_Resume@PLT
.LEHE4:
.L106:
	vzeroupper
	jmp	.L75
	.cfi_endproc
.LFE8901:
	.section	.gcc_except_table
.LLSDAC8901:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC8901-.LLSDACSBC8901
.LLSDACSBC8901:
	.uleb128 .LEHB4-.LCOLDB5
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSEC8901:
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
