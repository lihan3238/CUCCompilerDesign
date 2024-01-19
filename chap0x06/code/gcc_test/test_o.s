	.file	"test.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "return:%d\0"
.LC1:
	.ascii "pause\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	movl	$1, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	addl	$1, -4(%rbp)
	addl	$1, -8(%rbp)
.L2:
	cmpl	$9, -8(%rbp)
	jle	.L3
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC0(%rip), %rcx
	call	printf
	leaq	.LC1(%rip), %rcx
	call	system
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	system;	.scl	2;	.type	32;	.endef
