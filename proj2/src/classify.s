.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
	addi sp, sp, -68
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
    sw a0 48(sp)
    sw a1 52(sp)
    sw a2 56(sp)
    sw ra 60(sp)
    
    addi t0 x0 5
    bne a0 t0 wrong_arg_num
    
	# Read pretrained m0
    
    li a0 4
    jal malloc
    beq a0 x0 malloc_err1
    add s0 x0 a0 #s0 = int pointer for m0 rows
    
    
    li a0 4
    jal malloc
    beq a0 x0 malloc_err2
    add s1 x0 a0 #s1 = int pointer for m0 cols
    

    
    lw t0 52(sp) #setting t0 back to be a1
    lw a0 4(t0) #setting a0 to be the the file string for matrix 0
    add a1 s0 x0 #setting a1 to be the row pointer 
    add a2 s1 x0 #setting a2 to be the column pointer
    jal read_matrix
    add s2 x0 a0 #setting s2 to be the pointer to matrix m0
    
	# Read pretrained m1
    li a0 4
    jal malloc
    beq a0 x0 malloc_err3
    add s3 x0 a0 #s3 = int pointer for m1 rows
    
    li a0 4
    jal malloc
    beq a0 x0 malloc_err4
    add s4 x0 a0 #s4 = int pointer for m1 cols
    
    lw t0 52(sp) #setting t0 back to be a1
    lw a0 8(t0) #setting a0 to be the the file string for matrix 1
    add a1 s3 x0 #setting a1 to be the row pointer 
    add a2 s4 x0 #setting a2 to be the column pointer
    jal read_matrix
    add s5 x0 a0 #setting s5 to be the pointer to matrix m1

	# Read input matrix
    li a0 4
    jal malloc
    beq a0 x0 malloc_err5
    add s6 x0 a0 #s6 = int pointer for input rows
    
    li a0 4
    jal malloc
    beq a0 x0 malloc_err6
    add s7 x0 a0 #s7 = int pointer for input cols
    
    lw t0 52(sp) #setting t0 back to be a1
    lw a0 12(t0) #setting a0 to be the the file string for matrix input
    add a1 s6 x0 #setting a1 to be the row pointer 
    add a2 s7 x0 #setting a2 to be the column pointer
    jal read_matrix
    add s8 x0 a0 #setting s5 to be the pointer to matrix input
    
    #Creating Space for the the h matrix on the stack
    lw t0 0(s0) #set t0 to be the row size of m0
    lw t1 0(s7) #set t1 to be the column size of input
    mul t2 t0 t1 #number of elements h will have 
    slli t3 t2 2 #number of bytes that h will need
    sw t2 64(sp) #storing number of elements h will have as we need to use it in relu
    add a0 x0 t3 #setting a0 for malloc
    jal malloc
    beq x0 a0 malloc_err7
    add s9 a0 x0 #setting s9 to be the pointer to h
    
        
	# Compute h = matmul(m0, input)
    #   d = matmul(m0, m1)
	# Arguments:
	#   a0 (int*)  is the pointer to the start of m0
	#   a1 (int)   is the # of rows (height) of m0
	#   a2 (int)   is the # of columns (width) of m0
	#   a3 (int*)  is the pointer to the start of m1
	#   a4 (int)   is the # of rows (height) of m1
	#   a5 (int)   is the # of columns (width) of m1
	#   a6 (int*)  is the pointer to the the start of d
    add a0 s2 x0
    lw a1 0(s0)
    lw a2 0(s1)
    add a3 s8 x0
    lw a4 0(s6)
    lw a5 0(s7)
    add a6 s9 x0
    jal matmul
    
	# Compute h = relu(h)
    # Arguments:
	#   a0 (int*) is the pointer to the array
	#   a1 (int)  is the # of elements in the array
    add a0 x0 s9
    lw a1 64(sp)
    jal relu
 
	#Creating Space for the the o matrix on the stack
    lw t0 0(s3) #set t0 to be the row size of m1
    lw t1 0(s7) #set t1 to be the column size of h
    mul t2 t0 t1 #number of elements o will have 
    slli t3 t2 2 #number of bytes that o will need
    sw t2 64(sp) #storing number of elements o will have as we need to use it in argmax
    add a0 x0 t3 #setting a0 for malloc
    jal malloc
    beq x0 a0 malloc_err8
    add s10 a0 x0 #setting s10 to be the pointer to o
    
	# Compute o = matmul(m1, h)
    #   d = matmul(m0, m1)
	# Arguments:
	#   a0 (int*)  is the pointer to the start of m0
	#   a1 (int)   is the # of rows (height) of m0
	#   a2 (int)   is the # of columns (width) of m0
	#   a3 (int*)  is the pointer to the start of m1
	#   a4 (int)   is the # of rows (height) of m1
	#   a5 (int)   is the # of columns (width) of m1
	#   a6 (int*)  is the pointer to the the start of d
    add a0 s5 x0
    lw a1 0(s3)
    lw a2 0(s4)
    add a3 s9 x0
    lw a4 0(s0)
    lw a5 0(s7)
    add a6 s10 x0
    jal matmul
    
	# Write output matrix o
    lw t0 52(sp) #loading the original a1 into t0
   	# Arguments:
	#   a0 (char*) is the pointer to string representing the filename
	#   a1 (int*)  is the pointer to the start of the matrix in memory
	#   a2 (int)   is the number of rows in the matrix
	#   a3 (int)   is the number of columns in the matrix
    add a1 s10 x0
    lw a2 0(s3) 
    lw a3 0(s7) 
    
    lw a0 16(t0)
    jal write_matrix
    
	# Compute and return argmax(o)
    # Arguments:
	#   a0 (int*) is the pointer to the start of the array
	#   a1 (int)  is the # of elements in the array
    add a0 s10 x0
    lw a1 64(sp)
    jal argmax
    add s11 x0 a0 #putting the return value of argmax into s11 for the time being

	# If enabled, print argmax(o) and newline
    addi t0 x0 1
    lw a2 56(sp)
    beq a2 t0 free_all_memory 
    add a0 s11 x0
    jal print_int
    addi a0 x0 '\n'
    jal print_char
    
 free_all_memory:
 
 	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
    mv a0 s4			
    jal free
    mv a0 s5			
    jal free
    mv a0 s6			
    jal free
    mv a0 s7			
    jal free
    mv a0 s8			
    jal free
    mv a0 s9			
    jal free
    mv a0 s10			
    jal free
 	ebreak
    
 	mv a0 s11
    
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
    lw ra 60(sp)
    addi sp, sp, 68
    
	ret
    
wrong_arg_num:
	addi a0 x0 31 
    j Done
malloc_err1:
	addi a0 x0 26
    j Done
malloc_err2:
	mv a0 s0
    jal free			
	addi a0 x0 26
    j Done
malloc_err3:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
	addi a0 x0 26
    j Done
malloc_err4:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
	addi a0 x0 26
    j Done
malloc_err5:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
    mv a0 s4			
    jal free
    mv a0 s5			
    jal free
	addi a0 x0 26
    j Done
malloc_err6:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
    mv a0 s4			
    jal free
    mv a0 s5			
    jal free
    mv a0 s6			
    jal free
	addi a0 x0 26
    j Done
malloc_err7:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
    mv a0 s4			
    jal free
    mv a0 s5			
    jal free
    mv a0 s6			
    jal free
    mv a0 s7			
    jal free
    mv a0 s8			
    jal free
	addi a0 x0 26
    j Done
malloc_err8:
	mv a0 s0
    jal free			
 	mv a0 s1			
    jal free			
    mv a0 s2			
    jal free
    mv a0 s3			
    jal free
    mv a0 s4			
    jal free
    mv a0 s5			
    jal free
    mv a0 s6			
    jal free
    mv a0 s7			
    jal free
    mv a0 s8			
    jal free
    mv a0 s9			
    jal free
	addi a0 x0 26
    j Done
Done:
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
    sw ra 60(sp)
    addi sp, sp, 68
    j exit
    

 	

