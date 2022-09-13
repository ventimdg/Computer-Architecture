.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	blt x0 a1 loop_start
    li a0 36
    j exit
loop_start:
	addi sp sp -4
    sw a0 0(sp) #storing original address of array
    add t1 x0 x0 #t1 = curr, set to 0
    addi t3 x0 -1 #setting t3 to -1
loop_continue:
	beq a1 t1 loop_end #checking if we are at a valid address in the array
    lw t2 0(a0) #setting t2 to the current number we are at in the array
    blt t3 t2 skip #skipping setting num to zero if t2 > -1
    sw x0 0(a0) #if num is negative, setting that number to 0
skip:
	addi t1 t1 1 #increment index
    addi a0 a0 4 #increment address
    j loop_continue
loop_end:
	lw a0 0(sp)
    addi sp sp 4
	ret
