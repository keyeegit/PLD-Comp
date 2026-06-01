.globl _main
_main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #16
    mov w8, #42
    str w8, [x29, #-4]
    ldr w8, [x29, #-4]
    str w8, [x29, #-8]
    ldr w0, [x29, #-8]
    add sp, sp, #16
    ldp x29, x30, [sp], #16
    ret
