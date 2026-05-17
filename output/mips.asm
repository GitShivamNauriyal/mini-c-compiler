
================ GENERATING MIPS ASSEMBLY ================
.data
.text
.globl main

main:
  # Initialize stack pointer and frame pointer
  move $fp, $sp

  li $t0, 15  # Load constant
  sw $t0, 0($fp)  # Store to n
  li $t0, 0  # Load constant
  sw $t0, -4($fp)  # Store to first
  li $t0, 1  # Load constant
  sw $t0, -8($fp)  # Store to second
  li $t0, 0  # Load constant
  sw $t0, -12($fp)  # Store to c
  li $t0, 0
  li $t1, 15
  slt $t2, $t0, $t1
  sw $t2, -16($fp)
  li $t0, 0
  li $t1, 1
  sle $t2, $t0, $t1
  sw $t2, -20($fp)
  li $t0, 0  # Load constant
  sw $t0, -24($fp)  # Store to next
  li $t0, 1  # Load constant
  sw $t0, -24($fp)  # Store to next
  li $t0, 1  # Load constant
  sw $t0, -4($fp)  # Store to first
  li $t0, 0  # Load constant
  sw $t0, -8($fp)  # Store to second
  li $t0, 1  # Load constant
  sw $t0, -12($fp)  # Store to c
L1:
  lw $t0, -16($fp)
  bnez $t0, L2  # Branch if non-zero
  j L3
L2:
  lw $t0, -20($fp)
  bnez $t0, L4  # Branch if non-zero
  j L6
L4:
  j L5
L6:
L5:
  j L1
L3:

  # Exit Program (System Call 10)
  li $v0, 10
  syscall
==========================================================

