.globl _main
_main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #16
    mov w8, #1
    str w8, [x29, #-4]
    mov w8, #2
    str w8, [x29, #-8]
    ldr w8, [x29, #-8]
    str w8, [x29, #-12]
    ldr w0, [x29, #-12]
    add sp, sp, #16
    ldp x29, x30, [sp], #16
    ret
