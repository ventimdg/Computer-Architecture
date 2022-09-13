.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:
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
    
    add s0 a0 x0 #s0 = filename 
    add s1 a1 x0 #s1 = start of matrix  in memory
    add s2 a2 x0 #s2 = rows 
    add s3 a3 x0 #s3 = columns
    mul s4 s3 s2
    slli s4 s4 2 #s4 = number of bytes our array uses 
    
    addi a1 x0 1 #setting a1 to write permissions
    jal fopen
    addi t0 x0 -1 #set t0 to see if the file did not correctly open
    beq a0 t0 open_err #call open_err if file did not open correctly
    sw a0 40(sp) #store file descriptor because it will get overwrote later
    
    addi s5 x0 8 #set s5 to be the number of bytes for malloc
    add a0 s5 x0
    jal malloc
    add s5 a0 x0 #move the pointer to row and col to s5
    sw s2 0(s5) #storing row into pointer
    sw s3 4(s5) #storing column into pointer
    
    lw a0 40(sp) #set a0 to be the file descriptor
    add a1 s5 x0 #set a1 to be the pointer to our row and column
    addi a2 x0 2 #setting a2 to number of elements we are writing to the file
    addi a3 x0 4 #setting the size in bytes of each element
    jal fwrite
    addi t0 x0 2
    bne t0 a0 write_err
    
    lw a0 40(sp) #set a0 to be the file descriptor
    add a1 s1 x0 #set a1 to be the pointer to our matrix
    mul a2 s2 s3 #set a2 to number of elements we are writing in the file
    addi a3 x0 4 #setting the size in bytes of each element
    jal fwrite
    mul t0 s2 s3
    bne t0 a0 write_err
    
    lw a0 40(sp) #setting a0 to be the file descriptor
    jal fclose
    bne a0 x0 close_err
    
    mv a0 s5
    jal free 
    
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
write_err:
	addi a0 x0 30
    j Done
close_err:
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
