.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	blt x0 a1 loop_start
    li a0 36
    j exit
loop_start:
	add t1 x0 x0 #t1 is the starting index
	add t2 x0 x0 #t2 is the answer index, set to 0
    add t6 a0 x0 #holder for a0
    lw t3 0(a0) #the number corresponding the smallest index, initialized to the first element of the array
loop_continue:
    lw t4 0(t6) #current number we are on in the loop
    blt t4 t3 skip
    add t3 t4 x0 # set largest number to new number
    add t2 t1 x0 #set answer index to new index
skip: 
    addi t1 t1 1 # increment index
    beq t1 a1 loop_end
	addi t6 t6 4 #increment a0 to next int
    j loop_continue
loop_end:
	add a0 t2 x0
	ret
