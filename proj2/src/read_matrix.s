.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:
	# Prologue
    addi sp sp -44
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)	
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw ra 36(sp)
    
    add s1 x0 a1 #store pointer to the rows in s1
    add s2 x0 a2 #store pointers to the columns in s2
    add s3 x0 a0 #store pointer to the file name in s3
    
    add a1 x0 x0 #set File Permission for fopen
    jal fopen #call fopen
    
    addi t0 x0 -1 #set t0 to see if the file did not correctly open
    beq a0 t0 open_err #call open_err if file did not open correctly
    sw a0 40(sp) #store file descriptor because it will get overwrote later
    
    addi s4 x0 4 #set s4 to be the number of bytes we want to read each time
    lw a0 40(sp) #set a0 to be the file descriptor
    add a1 s1 x0 #set a1 to be the row pointer 
    add a2 s4 x0 #set a2 to the number of bytes we want to read
    jal fread
    bne s4 a0 read_err
    
    lw a0 40(sp) #set a0 to be the file descriptor
    add a1 s2 x0 #set a1 to be the col pointer 
    add a2 s4 x0 #set a2 to the number of bytes we want to read
    jal fread
    bne s4 a0 read_err
    
    lw t1 0(s1) #set t1 to rows
    lw t2 0(s2) #set t2 to columns
    mul s5 t1 t2 #s5 = row * cols = number of elements our arry has
    mul s6 s5 s4 #s6 = number of bytes our array will take up, will be used for malloc 
    
    add a0 x0 s6 #setting a0 to number of bytes for malloc
    jal malloc
    
    beq a0 x0 mall_err
    add s7 x0 a0 #s7 is the pointer to the begininng of the answer array
    
    lw a0 40(sp) #set a0 to be the file descriptor
    add a1 s7 x0 #setting a1 to point to our new matrix in s7
    add a2 x0 s6 #setting a2 to the number of bytes, used by malloc, which all of our matrix data takes up
    jal fread
    bne a0 s6 read_err_malloc
    
    lw a0 40(sp) #setting a0 to be the file descriptor
    jal fclose
    bne a0 x0 close_err
    
# Epilogue
	add a0 x0 s7
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)	
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw ra 36(sp)
    addi sp sp 44
	ret
    
open_err:
	addi a0 x0 27 
    j Done
read_err:
	addi a0 x0 29
    j Done
read_err_malloc:
    mv a0 s7
    jal free
    addi a0 x0 29
    j Done
mall_err:
	addi a0 x0 26
    j Done
close_err:
    mv a0 s7
    jal free
	addi a0 x0 28
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
    lw ra 36(sp)
    addi sp sp 44
    j exit
