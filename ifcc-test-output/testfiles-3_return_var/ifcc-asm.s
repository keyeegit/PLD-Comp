.globl _main
_main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #16
    mov w8, #8
    str w8, [x29, #-4]
    ldr w0, [x29, #-4]
    add sp, sp, #16
    ldp x29, x30, [sp], #16
    ret
