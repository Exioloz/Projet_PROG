.global main
.text
main:
    b end

skip:
    mov r2, #2
    mov r3, #3
    add r4, r3, r2, lsl #8
end:
