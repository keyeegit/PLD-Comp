.globl _printSpaces
_printSpaces:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #32
    str w0, [x29, #-4]
    mov w8, #0
    str w8, [x29, #-12]
    ldr w8, [x29, #-12]
    str w8, [x29, #-8]
.L0:
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-4]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-16]
    ldr w8, [x29, #-16]
    cbz w8, .L1
    mov w8, #32
    str w8, [x29, #-20]
    ldr w0, [x29, #-20]
    bl _putchar
    mov w8, #1
    str w8, [x29, #-24]
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-24]
    add w8, w8, w9
    str w8, [x29, #-28]
    ldr w8, [x29, #-28]
    str w8, [x29, #-8]
    b .L0
.L1:
    b .Lexit_printSpaces
.Lexit_printSpaces:
    add sp, sp, #32
    ldp x29, x30, [sp], #16
    ret
.globl _printStars
_printStars:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #32
    str w0, [x29, #-4]
    mov w8, #0
    str w8, [x29, #-12]
    ldr w8, [x29, #-12]
    str w8, [x29, #-8]
.L2:
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-4]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-16]
    ldr w8, [x29, #-16]
    cbz w8, .L3
    mov w8, #42
    str w8, [x29, #-20]
    ldr w0, [x29, #-20]
    bl _putchar
    mov w8, #1
    str w8, [x29, #-24]
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-24]
    add w8, w8, w9
    str w8, [x29, #-28]
    ldr w8, [x29, #-28]
    str w8, [x29, #-8]
    b .L2
.L3:
    b .Lexit_printStars
.Lexit_printStars:
    add sp, sp, #32
    ldp x29, x30, [sp], #16
    ret
.globl _drawDiamond
_drawDiamond:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #112
    str w0, [x29, #-4]
    mov w8, #0
    str w8, [x29, #-12]
    ldr w8, [x29, #-12]
    str w8, [x29, #-8]
.L4:
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-4]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-16]
    ldr w8, [x29, #-16]
    cbz w8, .L5
    mov w8, #1
    str w8, [x29, #-20]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-20]
    sub w8, w8, w9
    str w8, [x29, #-24]
    ldr w8, [x29, #-24]
    ldr w9, [x29, #-8]
    sub w8, w8, w9
    str w8, [x29, #-28]
    ldr w0, [x29, #-28]
    bl _printSpaces
    mov w8, #2
    str w8, [x29, #-32]
    ldr w8, [x29, #-32]
    ldr w9, [x29, #-8]
    mul w8, w8, w9
    str w8, [x29, #-36]
    mov w8, #1
    str w8, [x29, #-40]
    ldr w8, [x29, #-36]
    ldr w9, [x29, #-40]
    add w8, w8, w9
    str w8, [x29, #-44]
    ldr w0, [x29, #-44]
    bl _printStars
    mov w8, #10
    str w8, [x29, #-48]
    ldr w0, [x29, #-48]
    bl _putchar
    mov w8, #1
    str w8, [x29, #-52]
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-52]
    add w8, w8, w9
    str w8, [x29, #-56]
    ldr w8, [x29, #-56]
    str w8, [x29, #-8]
    b .L4
.L5:
    mov w8, #1
    str w8, [x29, #-60]
    ldr w8, [x29, #-60]
    str w8, [x29, #-8]
.L6:
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-4]
    cmp w8, w9
    cset w8, lt
    str w8, [x29, #-64]
    ldr w8, [x29, #-64]
    cbz w8, .L7
    ldr w0, [x29, #-8]
    bl _printSpaces
    mov w8, #2
    str w8, [x29, #-68]
    mov w8, #1
    str w8, [x29, #-72]
    ldr w8, [x29, #-4]
    ldr w9, [x29, #-72]
    sub w8, w8, w9
    str w8, [x29, #-76]
    ldr w8, [x29, #-76]
    ldr w9, [x29, #-8]
    sub w8, w8, w9
    str w8, [x29, #-80]
    ldr w8, [x29, #-68]
    ldr w9, [x29, #-80]
    mul w8, w8, w9
    str w8, [x29, #-84]
    mov w8, #1
    str w8, [x29, #-88]
    ldr w8, [x29, #-84]
    ldr w9, [x29, #-88]
    add w8, w8, w9
    str w8, [x29, #-92]
    ldr w0, [x29, #-92]
    bl _printStars
    mov w8, #10
    str w8, [x29, #-96]
    ldr w0, [x29, #-96]
    bl _putchar
    mov w8, #1
    str w8, [x29, #-100]
    ldr w8, [x29, #-8]
    ldr w9, [x29, #-100]
    add w8, w8, w9
    str w8, [x29, #-104]
    ldr w8, [x29, #-104]
    str w8, [x29, #-8]
    b .L6
.L7:
    b .Lexit_drawDiamond
.Lexit_drawDiamond:
    add sp, sp, #112
    ldp x29, x30, [sp], #16
    ret
.globl _main
_main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #16
    mov w8, #5
    str w8, [x29, #-4]
    ldr w0, [x29, #-4]
    bl _drawDiamond
    mov w8, #0
    str w8, [x29, #-8]
    ldr w0, [x29, #-8]
    b .Lexit_main
.Lexit_main:
    add sp, sp, #16
    ldp x29, x30, [sp], #16
    ret
