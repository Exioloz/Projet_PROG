.global main
.text
main:
branch_c: @test du flag C (retenue)
    mov r0,#1
    adds r0, #1 @addition with carry
    bcs branch_c @vérifie le flag C
branch_z: @test du flag Z (egalite avec 0)
    mov r0, #5
    subs r0, #1
    cmp r0, #3
    bne branch_z @vérifie le flag Z 
branch_n : @test du flag N (negatif) ????
    mov r0, #1
    mvn r0, r0
    bpl branch_n
branch_v: @test du flag V (debordement)
    mov r0, #255
    add r0, #3
    bvs branch_v
fin:
