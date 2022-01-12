.global main
.text
main:
    mov r1, #1
    mov r2, #2

    orr r1, r2
    and r1, r2
    mvn r1, r2
    and r1, r2
    xor r1, r2
