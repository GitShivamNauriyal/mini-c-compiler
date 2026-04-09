
================ GENERATING MIPS ASSEMBLY ================
.data
.text
.globl main

main:
  # Initialize stack pointer and frame pointer
  move $fp, $sp

  li $t0, 10  # Load constant
  sw $t0, 0($fp)  # Store to x
  li $t0, 3.140000  # Load constant
  sw $t0, -4($fp)  # Store to y
  li $t0, 10
  li $t1, 10
  seq $t2, $t0, $t1
  sw $t2, -8($fp)
  lw $t0, -8($fp)
  bnez $t0, L1  # Branch if non-zero
  j L3
L1:
  li $t0, 20  # Load constant
  sw $t0, 0($fp)  # Store to x
  j L2
L3:
  li $t0, 0.000000  # Load constant
  sw $t0, -4($fp)  # Store to y
L2:
  li $t0, 20
  li $t1, 0
  sgt $t2, $t0, $t1
  sw $t2, -12($fp)
  li $t0, 19  # Load constant
  sw $t0, 0($fp)  # Store to x
L4:
  lw $t0, -12($fp)
  bnez $t0, L5  # Branch if non-zero
  j L6
L5:
  j L4
L6:
  li $t0, 10  # Load constant
  sw $t0, -16($fp)  # Store to z

  # Exit Program (System Call 10)
  li $v0, 10
  syscall
==========================================================

