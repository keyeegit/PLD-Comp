.globl _isUpper
_isUpper:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #32
    str w0, [x29, #-4]
    mov w8, #64
    str w8, [x29, #-8]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-8]
    cmp w8, w9
    cset w8, gt
    str w8, [x29, #-12]
    mov w8, #91
    str w8, [x29, #-16]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-16]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-20]
    ldr w8, [x29, #-12]
    ldr w9, [x29, #-20]
    and w8, w8, w9
    str w8, [x29, #-24]
    ldr w0, [x29, #-24]
    b .Lexit_isUpper
.Lexit_isUpper:
    add sp, sp, #32
    ldp x29, x30, [sp], #16
    ret
.globl _isLower
_isLower:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #32
    str w0, [x29, #-4]
    mov w8, #96
    str w8, [x29, #-8]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-8]
    cmp w8, w9
    cset w8, gt
    str w8, [x29, #-12]
    mov w8, #123
    str w8, [x29, #-16]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-16]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-20]
    ldr w8, [x29, #-12]
    ldr w9, [x29, #-20]
    and w8, w8, w9
    str w8, [x29, #-24]
    ldr w0, [x29, #-24]
    b .Lexit_isLower
.Lexit_isLower:
    add sp, sp, #32
    ldp x29, x30, [sp], #16
    ret
.globl _shift
_shift:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #80
    str w0, [x29, #-4]
    str w1, [x29, #-8]
    ldr w0, [x29, #-4]
    bl _isUpper
    str w0, [x29, #-12]
    ldr w8, [x29, #-12]
    cbz w8, .L0
    mov w8, #65
    str w8, [x29, #-16]
    mov w8, #65
    str w8, [x29, #-20]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-20]
    sub w8, w8, w9
    str w8, [x29, #-24]
    ldr w8, [x29, #-24]
    ldr w9, [x29, #-8]
    add w8, w8, w9
    str w8, [x29, #-28]
    mov w8, #26
    str w8, [x29, #-32]
    ldr w8, [x29, #-28]
    ldr w9, [x29, #-32]
    sdiv w10, w8, w9
    msub w8, w10, w9, w8
    str w8, [x29, #-36]
    ldr w8, [x29, #-16]
    ldr w9, [x29, #-36]
    add w8, w8, w9
    str w8, [x29, #-40]
    ldr w0, [x29, #-40]
    b .Lexit_shift
.L0:
    ldr w0, [x29, #-4]
    bl _isLower
    str w0, [x29, #-44]
    ldr w8, [x29, #-44]
    cbz w8, .L2
    mov w8, #97
    str w8, [x29, #-48]
    mov w8, #97
    str w8, [x29, #-52]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-52]
    sub w8, w8, w9
    str w8, [x29, #-56]
    ldr w8, [x29, #-56]
    ldr w9, [x29, #-8]
    add w8, w8, w9
    str w8, [x29, #-60]
    mov w8, #26
    str w8, [x29, #-64]
    ldr w8, [x29, #-60]
    ldr w9, [x29, #-64]
    sdiv w10, w8, w9
    msub w8, w10, w9, w8
    str w8, [x29, #-68]
    ldr w8, [x29, #-48]
    ldr w9, [x29, #-68]
    add w8, w8, w9
    str w8, [x29, #-72]
    ldr w0, [x29, #-72]
    b .Lexit_shift
.L2:
    ldr w0, [x29, #-4]
    b .Lexit_shift
.Lexit_shift:
    add sp, sp, #80
    ldp x29, x30, [sp], #16
    ret
.globl _main
_main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #48
    mov w8, #3
    str w8, [x29, #-12]
    ldr w8, [x29, #-12]
    str w8, [x29, #-4]
    bl _getchar
    str w0, [x29, #-16]
    ldr w8, [x29, #-16]
    str w8, [x29, #-8]
.L4:
    mov w8, #1
    str w8, [x29, #-20]
    ldr w8, [x29, #-20]
    neg w8, w8
    str w8, [x29, #-24]
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-24]
    cmp w8, w9
    cset w8, ne
    str w8, [x29, #-28]
    ldr w8, [x29, #-28]
    cbz w8, .L5
    ldr w0, [x29, #-8]
    ldr w1, [x29, #-4]
    bl _shift
    str w0, [x29, #-32]
    ldr w0, [x29, #-32]
    bl _putchar
    bl _getchar
    str w0, [x29, #-36]
    ldr w8, [x29, #-36]
    str w8, [x29, #-8]
    b .L4
.L5:
    mov w8, #0
    str w8, [x29, #-40]
    ldr w0, [x29, #-40]
    b .Lexit_main
.Lexit_main:
    add sp, sp, #48
    ldp x29, x30, [sp], #16
    ret
