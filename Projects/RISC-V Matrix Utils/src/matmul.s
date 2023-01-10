.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:
	addi t1 x0 1 #setting t1 to one for comparason 
    blt a1 t1 error38
    blt a2 t1 error38
    blt a4 t1 error38
    blt a5 t1 error38
    bne a2 a4 error38
    
    addi sp sp -52
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw s11 44(sp)
    sw ra 48(sp)
    
    add s0 a0 x0 
    add s1 a1 x0
    add s2 a2 x0
    add s3 a3 x0
    add s4 a4 x0
    add s5 a5 x0
    add s6 a6 x0
    
    add s7 x0 x0 #setting number of outer loops to 0
    add s8 x0 x0 #setting number of inner loops to 0
    add s9 a0 x0 #holder for matrix 0
    add s10 a3 x0 #holder for matrix 1
    add s11 a6 x0 #holder for answer matrix to be altered
	j outer_loop_start
    
error38:
	li a0 38
    j exit
    
outer_loop_start:
    add a0 s9 x0 #putting matrix 0 in a0
    add a1 s10 x0 #putting matrix 1 in a1
    add a2 s2 x0 #number of elements to use 
    addi a3 x0 1 #stride of array 0
    add a4 s5 x0 #stride of array 1
    jal dot #calculating dot product of one row and column
    sw a0 0(s11) #storing result of dot product in new matrix
    addi s10 s10 4 #incrementing matrix 1 to use next column
    addi s7 s7 1 #incrementing outer loop counter
    beq s5 s7 inner_loop_start
    addi s11 s11 4 #incrementing answer matrix to use next box
    j outer_loop_start
inner_loop_start:
	addi s8 s8 1 #incrementing inner loop counter
	beq s1 s8 end #checking if we are done
    addi t5 x0 4 #setting temp variable for multiplication
    mul t6 t5 s5 #setting temp variable so we reset matrix 1 back to the begining 
    sub s10 s10 t6 #resetting matrix 1 to point back to the beginning
    mul t6 t5 s2 #setting temp variable so we reset matrix 0 to next row
    add s9 s9 t6 #incrementing matrix 0 to point to the next row
    add s7 x0 x0 #resetting outer loop counter back to 0
    addi s11 s11 4 #incrementing answer matrix to use next box
    j outer_loop_start
end:
    add a0 s0 x0 
    add a1 s1 x0
    add a2 s2 x0
    add a3 s3 x0
    add a4 s4 x0
    add a5 s5 x0
    add a6 s6 x0
    
    
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw s11 44(sp)
    lw ra 48(sp)
    addi sp sp 52
	ret
