##
##
##  Created by Matthew Kidder
##
##  (Prime Printing)
################### data segment ####################
    .data
.globl	outputMsg2
outputMsg2:	.asciiz     " "
.globl	outputMsg
outputMsg:	.asciiz		"\nThe first 100 prime numbers are:\n"

################### code segment ####################
    .text
.globl main

main:

    addu	$s7, $0, $ra	#save the return address in a global register

    li      $v0, 4          #print_str (system call 4)
    la      $a0, outputMsg  # takes the address of string as an argument
    syscall

    add     $s0, $0, $0     #Initialize iterator variable


While:
    beq     $s0, 542, Exit  #test from 0-542 (first 100 primes)
    add     $a0, $zero, $s0
    jal     prime
    beq     $v0, $zero, notPrime   #skip printing non prime numbers
    li      $v0, 1
    add     $a0, $0, $s0
    syscall                 #output prime number
    li      $v0, 4          #print a space
    la      $a0, outputMsg2
    syscall
    addi    $s0, $s0, 1
    j       While

Exit:
    #Usual stuff at the end of the main
    addu    $ra, $0, $s7    #restore the return address
    jr      $ra             #return to the main program
    add     $0, $0, $0      #nop

notPrime:
    addi    $s0, $s0, 1    #increment i without printing number
    j       While

.globl 	prime
prime:
    addi	$sp, $sp, -8	#make space on the stack for two items
    sw      $ra, 4($sp)     #save the return address on the stack
    sw      $a0, 0($sp)     #save the argument n on the stack
    slti    $t0, $a0, 2     #if 1 or 0, not prime
    beq     $t0, $zero, L1  #if n > 1 go to L1
    addi	$v0, $zero, 0	#otherwise return 0
    addi	$sp, $sp, 8     #(just pop the saved items off stack since no call
    jr      $ra             #and return)
L1:
    addi    $t3, $zero, 2   #start at lowest prime
For:
    slt     $t0, $t3, $a0   #while i <= argument
    beq     $t0, $zero, End
    div     $a0, $t3        #test divisibility
    mfhi    $t2
    beq     $t2, $zero, False   #if mod 0, not prime
    addi	$t3, $t3, 1
    j       For
End:
    lw      $a0, 0($sp)     #restore the value of argument n
    lw      $ra, 4($sp)     #restore the return address
    addi	$sp, $sp, 8     #release the save area on the stack
    addi    $v0, $zero, 1   #true if we arrive at this point, return 1
    jr      $ra
False:
    addi    $v0, $zero, 0
    jr      $ra
