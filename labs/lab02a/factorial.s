.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
	addi t1 x0 3 #set t1 to 3
	add t2 a0 x0 #set t2 to n
    addi t3 x0 1 #set t3 to 1
    blt t2 t1 True #if n < 3, return
    j False
True:
	jr ra
False:
	addi t2 t2 -1 #decrement n by 1
    beq t2 t3 True #if n = 1, we are done
    mul a0 a0 t2 #multiply current answer by n-1
    j False 
