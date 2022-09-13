.globl main

.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text

main:
    # BEGIN PROLOGUE
    addi sp, sp, -20
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw ra, 16(sp)
    # END PROLOGUE
    addi t0, x0, 0
    addi s0, x0, 0
    la s1, source
    la s2, dest
loop:
    slli s3, t0, 2 # set s3 to t0 * 4 (address of k)
    add t1, s1, s3 # t1 = source  + address
    lw t2, 0(t1) #t2 = source[k]
    beq t2, x0, exit # exit if source[k] = 0
    add a0, x0, t2 #a0 = source[k]
    addi sp, sp, -8 #allocate 8 more bytes to the stack
    sw t0, 0(sp) # store t0 (k)
    sw t2, 4(sp) #store  t2 source[k]
    jal fun #jump to fun
    lw t0, 0(sp) #t0 = k
    lw t2, 4(sp) #t2 = source[k]
    addi sp, sp, 8 #delete 8 stack bytes
    add t2, x0, a0 #source[k] = fun(source[k])
    add t3, s2, s3 #t3 = address of where to put new source[k]
    sw t2, 0(t3) #store new t2 into dest
    add s0, s0, t2 #increment sum by new source[k]
    addi t0, t0, 1 #increment k by 1
    jal x0, loop #return to loop, set x0 to address of exit
exit:
    add a0, x0, s0
    # BEGIN EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    # END EPILOGUE
	li a0, 10
    ecall

fun:
    addi t0, a0, 1 #t0 = source[k] + 1
    sub t1, x0, a0 #t1 = -source[k]
    mul a0, t0, t1 #a0 = -source[k] * (source[k] + 1)
    jr ra #return to the line after fun