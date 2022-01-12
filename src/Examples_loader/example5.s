.global main
.text
main:
    ldr r0, =donnee1
    ldr r0, =donnee2

.data
donnee1:
    .word 0x11223344
donnee2:
    .word 0x55667788
