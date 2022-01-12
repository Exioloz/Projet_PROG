.global main
.text
main:
    mov r0, #1
    mov r1, r0
    boucle:
    add r2, [r1,#1]
    cmp r1, #7
    beq boucle
    fin:
