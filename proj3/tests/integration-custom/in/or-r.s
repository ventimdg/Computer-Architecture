addi s0, x0, 2047
addi s1, x0, 255
addi t0, x0, -1
addi t1, x0, -2048
addi t2, x0, -255
or s0 s1 s0
or s1 s1 t0
or s0 t0 t1
or t0 t1 t2
or t1 t2 s0
or t2 s1 s0