.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	addi t1 x0 1 #setting t1 to 1 for length comparison
    blt a2 t1 end36
	blt a3 t1 end37
    blt a4 t1 end37
    j loop_start
end37:
	li a0 37
    j exit
end36:
	li a0 36
    j exit
loop_start:
	addi t2 x0 4 #setting t2 to 4 for multiplication purposes of next 2 steps
	mul t0 a3 t2 #number of bits to add to a0 when doing the next dot product
    mul t1 a4 t2 #number of bits to add to a1 when doing the next dot product
    add t2 x0 x0 #the iteration we ar on in our loop
    add t3 x0 x0 #the answer 
    add t4 x0 x0 #the temporary location we will store each iteration of the dot product
loop_continue:
	beq t2 a2 loop_end
	lw t5 0(a0) #current number in a0 we are on
    lw t6 0(a1) #current number in a1 we are on
    mul t4 t5 t6
    add t3 t3 t4
    add a0 a0 t0
    add a1 a1 t1
    addi t2 t2 1
    j loop_continue
loop_end:
	add a0 t3 x0
	# Epilogue
	ret
