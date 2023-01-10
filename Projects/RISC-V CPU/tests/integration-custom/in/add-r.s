
addi s0, x0, 2047
addi s1, x0, 255
addi t0, x0, -1
addi t1, x0, -2048
addi t2, x0, -255
add s0 s1 s0
add s1 s1 t0
add s1 t0 t1
add t0 t1 t2
add t1 t2 t2
add t2 t1 s0