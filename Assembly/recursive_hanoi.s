##
##
##  Created by Matthew Kidder
##
##  Part 3 (Towers of Hanoi)
################### data segment ####################
    .data
.globl	Prompt
Prompt:	.asciiz     "\nEnter number of disks>"
.globl	MoveDisk
MoveDisk:	.asciiz		"\nMove disk "
.globl	FromPeg
FromPeg:	.asciiz		" from peg "
.globl	ToPeg
ToPeg:	.asciiz		" to peg "
.globl	newline
newline:	.asciiz		"\n"

################### code segment ####################
    .text
.globl main

main:

    addu	$s7, $0, $ra	#save the return address in a global register

    add     $s1, $0, $0     #Initialize input variable and default args
    addi    $a1, $zero, 1
    addi    $a2, $zero, 2
    addi    $a3, $zero, 3
    li      $v0, 4
    la      $a0, Prompt     #print prompt
    syscall
    li      $v0, 5
    syscall
    add     $s1, $0, $v0    #The value of N has been read into $s1

    add	$a0, $0, $s1        #set the parameter to N for hanoi call
    jal	hanoi

    #Usual stuff at the end of the main
    addu    $ra, $0, $s7    #restore the return address
    jr      $ra             #return to the main program
    add     $0, $0, $0      #nop


.globl 	hanoi
hanoi:

    #Base case
    slti    $t0, $a0, 2     #if n = 1
    bne     $t0, $zero, base #branch

    #Create stack
    addi	$sp, $sp, -20	#make space on the stack for 5 items
    sw      $ra, 16($sp)	#save the return address on the stack
    sw      $a3, 12($sp)    #save argument values on stack
    sw      $a2, 8($sp)
    sw      $a1, 4($sp)
    sw      $a0, 0($sp)

    #First swap
    addi    $a0, $a0, -1    #n = n - 1
    add     $t0, $zero, $a2 #save 2 for swap
    add     $a2, $zero, $a3 #swap 3 and 2
    add     $a3, $zero, $t0 #move 3 onto 2
    jal     hanoi

    #Load saved values
    lw      $t0, 0($sp)     #restore the argument values, use temp
    lw      $a1, 4($sp)     #for a0 or print will bomb it
    lw      $a2, 8($sp)
    lw      $a3, 12($sp)

    #Printing
    li      $v0, 4          #print move
    la      $a0, MoveDisk
    syscall
    li      $v0, 1
    add     $a0, $0, $t0    #print n
    syscall
    li      $v0, 4          #print from
    la      $a0, FromPeg
    syscall
    li      $v0, 1
    add     $a0, $0, $a1
    syscall                 #print peg
    li      $v0, 4          #print to
    la      $a0, ToPeg
    syscall
    li      $v0, 1
    add     $a0, $0, $a2
    syscall                 #print peg
    li      $v0, 4          #print \n
    la      $a0, newline
    syscall

    #Load saved values(prepare for 2nd swap)
    lw      $a0, 0($sp)
    lw      $a1, 4($sp)
    lw      $a2, 8($sp)
    lw      $a3, 12($sp)

    #Second swaps
    addi    $a0, $t0, -1    #n = n - 1
    add     $t0, $zero, $a1 #save 1 for swap
    add     $a1, $zero, $a3 #swap 3 and 1
    add     $a3, $zero, $t0 #move 1 onto 3
    jal     hanoi

    #Return to "first part" of hanoi and restore stack
    lw      $ra, 16($sp)
    addi    $sp, $sp, 20
    jr      $ra

base:

    #base case reached, print and return
    li      $v0, 4          #print move
    la      $a0, MoveDisk
    syscall
    li      $v0, 1
    add     $a0, $0, $t0    #print n
    syscall
    li      $v0, 4          #print from
    la      $a0, FromPeg
    syscall
    li      $v0, 1
    add     $a0, $0, $a1
    syscall                 #print peg
    li      $v0, 4          #print to
    la      $a0, ToPeg
    syscall
    li      $v0, 1
    add     $a0, $0, $a2
    syscall                 #print peg
    li      $v0, 4          #print \n
    la      $a0, newline
    syscall
    jr      $ra
