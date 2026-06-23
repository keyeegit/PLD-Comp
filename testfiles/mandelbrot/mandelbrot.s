.globl renderMandelbrot
renderMandelbrot:
    pushq %rbp
    movq %rsp, %rbp
    subq $352, %rsp
    movl $78, -64(%rbp)
    movl -64(%rbp), %eax
    movl %eax, -4(%rbp)
    movl $36, -68(%rbp)
    movl -68(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $30, -72(%rbp)
    movl -72(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $0, -76(%rbp)
    movl -76(%rbp), %eax
    movl %eax, -16(%rbp)
.L0:
    movl -16(%rbp), %eax
    movl -8(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -80(%rbp)
    cmpl $0, -80(%rbp)
    je .L1
    movl $0, -84(%rbp)
    movl -84(%rbp), %eax
    movl %eax, -20(%rbp)
.L2:
    movl -20(%rbp), %eax
    movl -4(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -88(%rbp)
    cmpl $0, -88(%rbp)
    je .L3
    movl $8192, -92(%rbp)
    movl -92(%rbp), %eax
    negl %eax
    movl %eax, -96(%rbp)
    movl $10240, -100(%rbp)
    movl -20(%rbp), %eax
    movl -100(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -104(%rbp)
    movl -104(%rbp), %eax
    movl -4(%rbp), %ecx
    cltd
    idivl %ecx
    movl %eax, -108(%rbp)
    movl -96(%rbp), %eax
    movl -108(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -112(%rbp)
    movl -112(%rbp), %eax
    movl %eax, -24(%rbp)
    movl $4505, -116(%rbp)
    movl -116(%rbp), %eax
    negl %eax
    movl %eax, -120(%rbp)
    movl $9011, -124(%rbp)
    movl -16(%rbp), %eax
    movl -124(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -128(%rbp)
    movl -128(%rbp), %eax
    movl -8(%rbp), %ecx
    cltd
    idivl %ecx
    movl %eax, -132(%rbp)
    movl -120(%rbp), %eax
    movl -132(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -136(%rbp)
    movl -136(%rbp), %eax
    movl %eax, -28(%rbp)
    movl $0, -140(%rbp)
    movl -140(%rbp), %eax
    movl %eax, -32(%rbp)
    movl $0, -144(%rbp)
    movl -144(%rbp), %eax
    movl %eax, -36(%rbp)
    movl $0, -148(%rbp)
    movl -148(%rbp), %eax
    movl %eax, -40(%rbp)
    movl $0, -152(%rbp)
    movl -152(%rbp), %eax
    movl %eax, -44(%rbp)
    movl $0, -156(%rbp)
    movl -156(%rbp), %eax
    movl %eax, -48(%rbp)
.L4:
    movl -40(%rbp), %eax
    movl -12(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -160(%rbp)
    cmpl $0, -160(%rbp)
    je .L5
    movl $0, -164(%rbp)
    movl -44(%rbp), %eax
    movl -164(%rbp), %ecx
    cmpl %ecx, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -168(%rbp)
    cmpl $0, -168(%rbp)
    je .L6
    movl -32(%rbp), %eax
    movl -32(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -172(%rbp)
    movl $4096, -176(%rbp)
    movl -172(%rbp), %eax
    movl -176(%rbp), %ecx
    cltd
    idivl %ecx
    movl %eax, -180(%rbp)
    movl -180(%rbp), %eax
    movl %eax, -52(%rbp)
    movl -36(%rbp), %eax
    movl -36(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -184(%rbp)
    movl $4096, -188(%rbp)
    movl -184(%rbp), %eax
    movl -188(%rbp), %ecx
    cltd
    idivl %ecx
    movl %eax, -192(%rbp)
    movl -192(%rbp), %eax
    movl %eax, -56(%rbp)
    movl -52(%rbp), %eax
    movl -56(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -196(%rbp)
    movl $16384, -200(%rbp)
    movl -196(%rbp), %eax
    movl -200(%rbp), %ecx
    cmpl %ecx, %eax
    setg %al
    movzbl %al, %eax
    movl %eax, -204(%rbp)
    cmpl $0, -204(%rbp)
    je .L8
    movl $1, -208(%rbp)
    movl -208(%rbp), %eax
    movl %eax, -44(%rbp)
    movl -40(%rbp), %eax
    movl %eax, -48(%rbp)
    jmp .L9
.L8:
    movl $2, -212(%rbp)
    movl -212(%rbp), %eax
    movl -32(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -216(%rbp)
    movl -216(%rbp), %eax
    movl -36(%rbp), %ecx
    imull %ecx, %eax
    movl %eax, -220(%rbp)
    movl $4096, -224(%rbp)
    movl -220(%rbp), %eax
    movl -224(%rbp), %ecx
    cltd
    idivl %ecx
    movl %eax, -228(%rbp)
    movl -228(%rbp), %eax
    movl -28(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -232(%rbp)
    movl -232(%rbp), %eax
    movl %eax, -60(%rbp)
    movl -52(%rbp), %eax
    movl -56(%rbp), %ecx
    subl %ecx, %eax
    movl %eax, -236(%rbp)
    movl -236(%rbp), %eax
    movl -24(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -240(%rbp)
    movl -240(%rbp), %eax
    movl %eax, -32(%rbp)
    movl -60(%rbp), %eax
    movl %eax, -36(%rbp)
.L9:
.L6:
    movl $1, -244(%rbp)
    movl -40(%rbp), %eax
    movl -244(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -248(%rbp)
    movl -248(%rbp), %eax
    movl %eax, -40(%rbp)
    jmp .L4
.L5:
    movl $0, -252(%rbp)
    movl -44(%rbp), %eax
    movl -252(%rbp), %ecx
    cmpl %ecx, %eax
    sete %al
    movzbl %al, %eax
    movl %eax, -256(%rbp)
    cmpl $0, -256(%rbp)
    je .L10
    movl $35, -260(%rbp)
    movl -260(%rbp), %edi
    call putchar
    jmp .L11
.L10:
    movl $3, -264(%rbp)
    movl -48(%rbp), %eax
    movl -264(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -268(%rbp)
    cmpl $0, -268(%rbp)
    je .L12
    movl $32, -272(%rbp)
    movl -272(%rbp), %edi
    call putchar
    jmp .L13
.L12:
    movl $6, -276(%rbp)
    movl -48(%rbp), %eax
    movl -276(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -280(%rbp)
    cmpl $0, -280(%rbp)
    je .L14
    movl $46, -284(%rbp)
    movl -284(%rbp), %edi
    call putchar
    jmp .L15
.L14:
    movl $10, -288(%rbp)
    movl -48(%rbp), %eax
    movl -288(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -292(%rbp)
    cmpl $0, -292(%rbp)
    je .L16
    movl $45, -296(%rbp)
    movl -296(%rbp), %edi
    call putchar
    jmp .L17
.L16:
    movl $15, -300(%rbp)
    movl -48(%rbp), %eax
    movl -300(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -304(%rbp)
    cmpl $0, -304(%rbp)
    je .L18
    movl $43, -308(%rbp)
    movl -308(%rbp), %edi
    call putchar
    jmp .L19
.L18:
    movl $22, -312(%rbp)
    movl -48(%rbp), %eax
    movl -312(%rbp), %ecx
    cmpl %ecx, %eax
    setl %al
    movzbl %al, %eax
    movl %eax, -316(%rbp)
    cmpl $0, -316(%rbp)
    je .L20
    movl $42, -320(%rbp)
    movl -320(%rbp), %edi
    call putchar
    jmp .L21
.L20:
    movl $37, -324(%rbp)
    movl -324(%rbp), %edi
    call putchar
.L21:
.L19:
.L17:
.L15:
.L13:
.L11:
    movl $1, -328(%rbp)
    movl -20(%rbp), %eax
    movl -328(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -332(%rbp)
    movl -332(%rbp), %eax
    movl %eax, -20(%rbp)
    jmp .L2
.L3:
    movl $10, -336(%rbp)
    movl -336(%rbp), %edi
    call putchar
    movl $1, -340(%rbp)
    movl -16(%rbp), %eax
    movl -340(%rbp), %ecx
    addl %ecx, %eax
    movl %eax, -344(%rbp)
    movl -344(%rbp), %eax
    movl %eax, -16(%rbp)
    jmp .L0
.L1:
    movl $0, %eax
.Lexit_renderMandelbrot:
    addq $352, %rsp
    popq %rbp
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    call renderMandelbrot
    movl $0, -4(%rbp)
    movl -4(%rbp), %eax
    jmp .Lexit_main
.Lexit_main:
    addq $16, %rsp
    popq %rbp
    ret
