.global main
.text
main:
    sub sp, sp, #4
    str lr,[sp]

    sub sp,sp,#4
    str fp,[sp]

    mov fp,sp

    push {r0-r2}
    ldr r0,[fp,#8]
    cmp r0,#0
    beq fin

    mov r2,r0,lsr#1
    sub sp,sp,#4
    str r2,[sp]
    add sp,sp,#4

    fin: pop {r0-r2}
    ldr fp,[sp]
    add sp,sp,#4
    ldr lr,[sp]
    add sp,sp,#4
