	.file	"s-talk.c"
	.text
	.globl	activeSession
	.bss
	.type	activeSession, @object
	.size	activeSession, 1
activeSession:
	.zero	1
	.globl	sendListEmpty
	.data
	.type	sendListEmpty, @object
	.size	sendListEmpty, 1
sendListEmpty:
	.byte	1
	.globl	printListEmpty
	.type	printListEmpty, @object
	.size	printListEmpty, 1
printListEmpty:
	.byte	1
	.comm	socketFD,4,4
	.comm	sendList,8,8
	.comm	printScreenList,8,8
	.comm	threadBuffer,32,32
	.globl	printListLock
	.bss
	.align 32
	.type	printListLock, @object
	.size	printListLock, 40
printListLock:
	.zero	40
	.globl	sendListLock
	.align 32
	.type	sendListLock, @object
	.size	sendListLock, 40
sendListLock:
	.zero	40
	.globl	sendListFullCond
	.align 32
	.type	sendListFullCond, @object
	.size	sendListFullCond, 48
sendListFullCond:
	.zero	48
	.globl	printListFullCond
	.align 32
	.type	printListFullCond, @object
	.size	printListFullCond, 48
printListFullCond:
	.zero	48
	.globl	printListEmptyCond
	.align 32
	.type	printListEmptyCond, @object
	.size	printListEmptyCond, 48
printListEmptyCond:
	.zero	48
	.globl	sendListEmptyCond
	.align 32
	.type	sendListEmptyCond, @object
	.size	sendListEmptyCond, 48
sendListEmptyCond:
	.zero	48
	.text
	.globl	itemFree
	.type	itemFree, @function
itemFree:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	$0, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	itemFree, .-itemFree
	.globl	initiateLists
	.type	initiateLists, @function
initiateLists:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	ListCreate@PLT
	movq	%rax, sendList(%rip)
	movl	$0, %eax
	call	ListCreate@PLT
	movq	%rax, printScreenList(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	initiateLists, .-initiateLists
	.section	.rodata
.LC0:
	.string	"getaddrinfo: %s\n"
	.align 8
.LC1:
	.string	"s-talk: socket creation failed"
.LC2:
	.string	"s-talk: socket bind failed"
.LC3:
	.string	"s-talk: socket failed to bind"
	.text
	.globl	setupLocal
	.type	setupLocal, @function
setupLocal:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%rdi, -104(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-64(%rbp), %rax
	movl	$48, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movl	$2, -60(%rbp)
	movl	$2, -56(%rbp)
	movl	$1, -64(%rbp)
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -88(%rbp)
	movq	-104(%rbp), %rdx
	leaq	-14(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	leaq	-80(%rbp), %rcx
	leaq	-64(%rbp), %rdx
	leaq	-14(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	getaddrinfo@PLT
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L4
	movl	-84(%rbp), %eax
	movl	%eax, %edi
	call	gai_strerror@PLT
	movq	%rax, %rdx
	movq	stderr(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
.L4:
	movq	-80(%rbp), %rax
	movq	%rax, -72(%rbp)
	jmp	.L5
.L12:
	movq	-72(%rbp), %rax
	movl	12(%rax), %edx
	movq	-72(%rbp), %rax
	movl	8(%rax), %ecx
	movq	-72(%rbp), %rax
	movl	4(%rax), %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	socket@PLT
	movl	%eax, socketFD(%rip)
	movl	socketFD(%rip), %eax
	cmpl	$-1, %eax
	jne	.L6
	leaq	.LC1(%rip), %rdi
	call	perror@PLT
	jmp	.L7
.L6:
	movq	-72(%rbp), %rax
	movl	16(%rax), %edx
	movq	-72(%rbp), %rax
	movq	24(%rax), %rcx
	movl	socketFD(%rip), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	bind@PLT
	cmpl	$-1, %eax
	jne	.L8
	movl	socketFD(%rip), %eax
	movl	%eax, %edi
	call	close@PLT
	leaq	.LC2(%rip), %rdi
	call	perror@PLT
	jmp	.L7
.L8:
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movzwl	(%rax), %eax
	cmpw	$2, %ax
	jne	.L16
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	freeaddrinfo@PLT
	movq	-72(%rbp), %rax
	jmp	.L14
.L7:
	movq	-72(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, -72(%rbp)
.L5:
	cmpq	$0, -72(%rbp)
	jne	.L12
	jmp	.L11
.L16:
	nop
.L11:
	cmpq	$0, -72(%rbp)
	jne	.L13
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$29, %edx
	movl	$1, %esi
	leaq	.LC3(%rip), %rdi
	call	fwrite@PLT
.L13:
	movl	$0, %eax
.L14:
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L15
	call	__stack_chk_fail@PLT
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	setupLocal, .-setupLocal
	.section	.rodata
.LC4:
	.string	"IP ADDRESS OF REMOTE: %s\n"
	.text
	.globl	setupRemote
	.type	setupRemote, @function
setupRemote:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$176, %rsp
	movq	%rdi, -168(%rbp)
	movq	%rsi, -176(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-112(%rbp), %rax
	movl	$48, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movl	$2, -108(%rbp)
	movl	$2, -104(%rbp)
	movq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -152(%rbp)
	movq	-176(%rbp), %rdx
	leaq	-54(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movq	-168(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	leaq	-144(%rbp), %rcx
	leaq	-112(%rbp), %rdx
	leaq	-54(%rbp), %rsi
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	getaddrinfo@PLT
	movl	%eax, -148(%rbp)
	cmpl	$0, -148(%rbp)
	je	.L18
	movl	-148(%rbp), %eax
	movl	%eax, %edi
	call	gai_strerror@PLT
	movq	%rax, %rdx
	movq	stderr(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
.L18:
	movq	-144(%rbp), %rax
	movq	%rax, -136(%rbp)
	jmp	.L19
.L22:
	movq	-136(%rbp), %rax
	movq	24(%rax), %rax
	movzwl	(%rax), %eax
	cmpw	$2, %ax
	jne	.L20
	movq	-136(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -128(%rbp)
	movq	-128(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, %edi
	call	inet_ntoa@PLT
	movq	%rax, -120(%rbp)
	movq	-120(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-144(%rbp), %rax
	movq	%rax, %rdi
	call	freeaddrinfo@PLT
	movq	-136(%rbp), %rax
	jmp	.L17
.L20:
	movq	-136(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, -136(%rbp)
.L19:
	cmpq	$0, -136(%rbp)
	jne	.L22
.L17:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L23
	call	__stack_chk_fail@PLT
.L23:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	setupRemote, .-setupRemote
	.section	.rodata
.LC5:
	.string	"Enter a message: "
	.align 8
.LC6:
	.string	"! inputted.  Terminating session now..."
	.text
	.globl	awaitInput
	.type	awaitInput, @function
awaitInput:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1056, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movb	$33, -1041(%rbp)
	call	pthread_testcancel@PLT
	jmp	.L25
.L33:
	call	pthread_testcancel@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L26
.L27:
	call	pthread_testcancel@PLT
	leaq	sendListLock(%rip), %rsi
	leaq	sendListEmptyCond(%rip), %rdi
	call	pthread_cond_wait@PLT
.L26:
	movzbl	sendListEmpty(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	jne	.L27
	jmp	.L28
.L32:
	call	pthread_testcancel@PLT
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
	leaq	-1040(%rbp), %rax
	movl	$1024, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	stdin(%rip), %rdx
	leaq	-1040(%rbp), %rax
	movl	$1024, %esi
	movq	%rax, %rdi
	call	fgets@PLT
	leaq	-1040(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	subq	$1, %rax
	movb	$0, -1040(%rbp,%rax)
	movzbl	-1040(%rbp), %eax
	cmpb	%al, -1041(%rbp)
	jne	.L29
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movl	$1, %edi
	call	sleep@PLT
	movq	sendList(%rip), %rax
	leaq	-1040(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ListAdd@PLT
	movb	$0, sendListEmpty(%rip)
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	sendListFullCond(%rip), %rdi
	call	pthread_cond_signal@PLT
	movq	24+threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movb	$0, activeSession(%rip)
	leaq	-1040(%rbp), %rax
	movl	$1024, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movl	socketFD(%rip), %eax
	movl	%eax, %edi
	call	close@PLT
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L30
	movq	sendList(%rip), %rax
	leaq	itemFree(%rip), %rsi
	movq	%rax, %rdi
	call	ListFree@PLT
.L30:
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L31
	movq	printScreenList(%rip), %rax
	leaq	itemFree(%rip), %rsi
	movq	%rax, %rdi
	call	ListFree@PLT
.L31:
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	leaq	sendListFullCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	printListFullCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	sendListEmptyCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	printListEmptyCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	movq	8+threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movq	16+threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movl	$0, %edi
	call	pthread_exit@PLT
.L29:
	movq	sendList(%rip), %rax
	leaq	-1040(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ListAdd@PLT
	movb	$0, sendListEmpty(%rip)
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	sendListFullCond(%rip), %rdi
	call	pthread_cond_signal@PLT
.L28:
	movzbl	sendListEmpty(%rip), %eax
	testb	%al, %al
	jne	.L32
.L25:
	movzbl	activeSession(%rip), %eax
	testb	%al, %al
	jne	.L33
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE9:
	.size	awaitInput, .-awaitInput
	.section	.rodata
	.align 8
.LC7:
	.string	"! received from remote user.  Terminating session now..."
	.text
	.globl	awaitUDP
	.type	awaitUDP, @function
awaitUDP:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1072, %rsp
	movq	%rdi, -1064(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-1064(%rbp), %rax
	movq	%rax, -1048(%rbp)
	movl	$8, -1052(%rbp)
	movb	$33, -1053(%rbp)
	call	pthread_testcancel@PLT
	jmp	.L36
.L44:
	call	pthread_testcancel@PLT
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L37
.L38:
	call	pthread_testcancel@PLT
	leaq	printListLock(%rip), %rsi
	leaq	printListEmptyCond(%rip), %rdi
	call	pthread_cond_wait@PLT
.L37:
	movzbl	printListEmpty(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	jne	.L38
	jmp	.L39
.L43:
	call	pthread_testcancel@PLT
	movl	socketFD(%rip), %eax
	leaq	-1052(%rbp), %rcx
	movq	-1048(%rbp), %rdx
	leaq	-1040(%rbp), %rsi
	movq	%rcx, %r9
	movq	%rdx, %r8
	movl	$0, %ecx
	movl	$1023, %edx
	movl	%eax, %edi
	call	recvfrom@PLT
	movzbl	-1040(%rbp), %eax
	cmpb	%al, -1053(%rbp)
	jne	.L40
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	movl	$1, %edi
	call	sleep@PLT
	movb	$0, activeSession(%rip)
	leaq	-1040(%rbp), %rax
	movl	$1024, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movl	socketFD(%rip), %eax
	movl	%eax, %edi
	call	close@PLT
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L41
	movq	sendList(%rip), %rax
	leaq	itemFree(%rip), %rsi
	movq	%rax, %rdi
	call	ListFree@PLT
.L41:
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L42
	movq	printScreenList(%rip), %rax
	leaq	itemFree(%rip), %rsi
	movq	%rax, %rdi
	call	ListFree@PLT
.L42:
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	leaq	sendListFullCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	printListFullCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	sendListEmptyCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	leaq	printListEmptyCond(%rip), %rdi
	call	pthread_cond_destroy@PLT
	movq	threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movq	16+threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movq	24+threadBuffer(%rip), %rax
	movq	%rax, %rdi
	call	pthread_cancel@PLT
	movl	$0, %edi
	call	pthread_exit@PLT
.L40:
	movq	printScreenList(%rip), %rax
	leaq	-1040(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ListAdd@PLT
	movb	$0, printListEmpty(%rip)
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	printListFullCond(%rip), %rdi
	call	pthread_cond_signal@PLT
.L39:
	movzbl	printListEmpty(%rip), %eax
	testb	%al, %al
	jne	.L43
.L36:
	movzbl	activeSession(%rip), %eax
	testb	%al, %al
	jne	.L44
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE10:
	.size	awaitUDP, .-awaitUDP
	.section	.rodata
.LC8:
	.string	"msg received: %s\n"
	.text
	.globl	printScreen
	.type	printScreen, @function
printScreen:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1040, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	call	pthread_testcancel@PLT
	jmp	.L47
.L52:
	call	pthread_testcancel@PLT
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L48
.L49:
	call	pthread_testcancel@PLT
	leaq	printListLock(%rip), %rsi
	leaq	printListFullCond(%rip), %rdi
	call	pthread_cond_wait@PLT
.L48:
	movzbl	printListEmpty(%rip), %eax
	testb	%al, %al
	jne	.L49
	jmp	.L50
.L51:
	call	pthread_testcancel@PLT
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L50
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListFirst@PLT
	testq	%rax, %rax
	je	.L50
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListCurr@PLT
	testq	%rax, %rax
	je	.L50
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListFirst@PLT
	movq	%rax, %rdx
	leaq	-1040(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movq	printScreenList(%rip), %rax
	movq	%rax, %rdi
	call	ListRemove@PLT
	leaq	printListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	printListEmptyCond(%rip), %rdi
	call	pthread_cond_signal@PLT
	movb	$1, printListEmpty(%rip)
	leaq	-1040(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L50:
	movzbl	printListEmpty(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	jne	.L51
.L47:
	movzbl	activeSession(%rip), %eax
	testb	%al, %al
	jne	.L52
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE11:
	.size	printScreen, .-printScreen
	.section	.rodata
	.align 8
.LC9:
	.string	"sendUDP: failed to send UDP datagram to remote process"
	.text
	.globl	sendUDP
	.type	sendUDP, @function
sendUDP:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1072, %rsp
	movq	%rdi, -1064(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-1064(%rbp), %rax
	movq	%rax, -1048(%rbp)
	movl	$16, -1056(%rbp)
	call	pthread_testcancel@PLT
	jmp	.L55
.L60:
	call	pthread_testcancel@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L56
.L57:
	call	pthread_testcancel@PLT
	leaq	sendListLock(%rip), %rsi
	leaq	sendListFullCond(%rip), %rdi
	call	pthread_cond_wait@PLT
.L56:
	movzbl	sendListEmpty(%rip), %eax
	testb	%al, %al
	jne	.L57
	jmp	.L58
.L59:
	call	pthread_testcancel@PLT
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListCount@PLT
	testl	%eax, %eax
	jle	.L58
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListFirst@PLT
	testq	%rax, %rax
	je	.L58
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListCurr@PLT
	testq	%rax, %rax
	je	.L58
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListFirst@PLT
	movq	%rax, %rdx
	leaq	-1040(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movq	sendList(%rip), %rax
	movq	%rax, %rdi
	call	ListRemove@PLT
	movb	$1, sendListEmpty(%rip)
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	leaq	sendListEmptyCond(%rip), %rdi
	call	pthread_cond_signal@PLT
	leaq	sendListLock(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	movl	socketFD(%rip), %eax
	movl	-1056(%rbp), %ecx
	movq	-1064(%rbp), %rdx
	leaq	-1040(%rbp), %rsi
	movl	%ecx, %r9d
	movq	%rdx, %r8
	movl	$0, %ecx
	movl	$1024, %edx
	movl	%eax, %edi
	call	sendto@PLT
	cmpq	$-1, %rax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -1052(%rbp)
	cmpl	$0, -1052(%rbp)
	je	.L58
	leaq	.LC9(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L58:
	movzbl	sendListEmpty(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	jne	.L59
.L55:
	movzbl	activeSession(%rip), %eax
	testb	%al, %al
	jne	.L60
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE12:
	.size	sendUDP, .-sendUDP
	.section	.rodata
	.align 8
.LC10:
	.string	"usage: s-talk myPortNumber remoteMachine remotePortNumber\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$176, %rsp
	movl	%edi, -164(%rbp)
	movq	%rsi, -176(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$4, -164(%rbp)
	je	.L63
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$58, %edx
	movl	$1, %esi
	leaq	.LC10(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L63:
	movl	$0, %eax
	call	initiateLists
	movq	-176(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -152(%rbp)
	movq	-176(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -148(%rbp)
	movq	-176(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	setupLocal
	movq	%rax, %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -144(%rbp)
	movq	%rdx, -136(%rbp)
	movq	16(%rcx), %rax
	movq	24(%rcx), %rdx
	movq	%rax, -128(%rbp)
	movq	%rdx, -120(%rbp)
	movq	32(%rcx), %rax
	movq	40(%rcx), %rdx
	movq	%rax, -112(%rbp)
	movq	%rdx, -104(%rbp)
	movq	-176(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rdx
	movq	-176(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	setupRemote
	movq	%rax, %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movq	16(%rcx), %rax
	movq	24(%rcx), %rdx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	32(%rcx), %rax
	movq	40(%rcx), %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-120(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movb	$1, activeSession(%rip)
	movl	$0, %ecx
	leaq	awaitInput(%rip), %rdx
	movl	$0, %esi
	leaq	threadBuffer(%rip), %rdi
	call	pthread_create@PLT
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	leaq	awaitUDP(%rip), %rdx
	movl	$0, %esi
	leaq	8+threadBuffer(%rip), %rdi
	call	pthread_create@PLT
	movl	$0, %ecx
	leaq	printScreen(%rip), %rdx
	movl	$0, %esi
	leaq	16+threadBuffer(%rip), %rdi
	call	pthread_create@PLT
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	leaq	sendUDP(%rip), %rdx
	movl	$0, %esi
	leaq	24+threadBuffer(%rip), %rdi
	call	pthread_create@PLT
	movq	threadBuffer(%rip), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	8+threadBuffer(%rip), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	16+threadBuffer(%rip), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	24+threadBuffer(%rip), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	socketFD(%rip), %eax
	movl	%eax, %edi
	call	close@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
