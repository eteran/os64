
#define ASM
#include "multiboot.h"
#include "boot.h"

.globl  kmain

.code32
.section .multiboot
.align 4
multiboot_header:
	.long MULTIBOOT_HEADER_MAGIC                             /* magic */
	.long MULTIBOOT_HEADER_FLAGS                             /* flags */
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) /* checksum */
	.long (multiboot_header - KERNEL_VMA)                    /* header_addr */
	.long (_start - KERNEL_VMA)                              /* load_addr */
	.long (_edata - KERNEL_VMA)                              /* load_end_addr */
	.long (_end - KERNEL_VMA)                                /* bss_end_addr */
	.long (bootstrap - KERNEL_VMA)                           /* entry_addr */


.globl bootstrap, init_pml4, init_pdp, init_pd
bootstrap:
	/* make sure interrupts are off */
	cli

	/* load the GDT */
	lgdt (init_gdt64_ptr - KERNEL_VMA)

	/* give ourselves a functional stack */
	movl $(init_stack_end - KERNEL_VMA), %esp

	/* reset EFLAGS. */
	pushl $0
	popf

	/* store the multiboot info */
	pushl %eax
	pushl %ebx

	/* zero out bss section */
	xorl	%eax, %eax
	movl	$(_bss - KERNEL_VMA), %edi
	movl	$(_end - KERNEL_VMA), %ecx
	subl	%edi, %ecx
	cld
	rep	stosb

	/* get the multiboot info back */
	popl %esi
	popl %edi

	/* ensure that paging is off */
	movl	%cr0, %eax
	andl	$0x7fffffff, %eax
	movl	%eax, %cr0

	/* enable CR4.PAE */
	movl	%cr4, %eax
	orl		$0x20, %eax
	movl	%eax, %cr4

	/* load a level4 PD */
	movl	$(init_pml4 - KERNEL_VMA), %eax
	mov		%eax, %cr3

	/* set IA32_EFER.LME */
	movl	$0xc0000080, %ecx
	rdmsr
	/* set the PAE and SYSENTER bits */
	orl		$0x00000101, %eax
	wrmsr

	/* enable paging */
	movl	%cr0, %eax
	orl		$0x80000000, %eax
	movl	%eax, %cr0

	/* at this point we should be in IA-32e mode */
	ljmp $0x08, $(boot64 - KERNEL_VMA)

.data
.align 16
gdt64:
	.quad 0x0000000000000000 // 0x00 NULL
	.quad 0x0020980000000000 // 0x08 KCODE64
gdt64_end:

.align 16
init_gdt64_ptr:
	.word gdt64_end - gdt64 - 1
	.long gdt64 - KERNEL_VMA

/* ident map of first 4MB GB, and mirror it at the -2GB mark  */
.align 0x1000
init_pml4:
	.quad init_pdp - KERNEL_VMA + 3 // [0x0000000000000000 - 0x00000007ffffffff)
	.fill 510,8,0
	.quad init_pdp - KERNEL_VMA + 3 // [0xfffffff800000000 - 0xffffffffffffffff)

init_pdp:
	.quad init_pd - KERNEL_VMA + 3
	.fill 509,8,0
	.quad init_pd - KERNEL_VMA + 3
	.fill 1,8,0

init_pd:
	.quad 0x0000000000000083 // 0MB - 2MB
	.quad 0x0000000000200083 // 2MB - 4MB
	.fill 510,8,0


init_stack_start:
	.fill 0x1000,1,0
init_stack_end:


.code64
boot64_high:
	/* adjust the stack pointer to be in upper memory */
	movq $KERNEL_VMA, %rax
	addq %rax, %rsp

	/* go to our C entry point */
	call EXT_C(kmain)

boot64_hang:
	hlt
	jmp boot64_hang

boot64:
	/* do an indirect to force non-relative jmp */
	movabsq $boot64_high, %rax
	jmp *%rax


