##
##
##  Created by Matthew Kidder
##
##  (Adding Machine)
################### data segment ####################
    .data
.globl  inputNmsg
inputNmsg:  .asciiz   "\nEnter any integers then 0 to see the sum :"
.globl	outputMsg
outputMsg:	.asciiz   "\nSum = "

################### code segment ####################
    .text
.globl main

main:

    addu	$s7, $0, $ra	#save the return address in a global register

    li      $v0, 4          #print_str (system call 4)
    la      $a0, inputNmsg  # takes the address of string as an argument
    syscall
    li      $v0, 5
    syscall

    add     $s0, $0, $0     #Initialize storage variable
    add     $s1, $0, $v0    #The value of N has been read into $s1
    add     $s0, $s0, $s1   #Update value of running total

While:
    beq     $s1, 0, Exit
    li      $v0, 5          #Continue input until 0 entered
    syscall
    add     $s1, $0, $v0    #The value of N has been read into $s1
    add     $s0, $s0, $s1    #Update value of running total
    j       While

    #Output the result
Exit:
    li      $v0, 4          #print_str (system call 4)
    la      $a0, outputMsg  #takes the address of string as an argument
    syscall                 #output the label
    li      $v0, 1
    add     $a0, $0, $s0
    syscall                 #output total


    #Usual stuff at the end of the main
    addu    $ra, $0, $s7    #restore the return address
    jr      $ra             #return to the main program
    add     $0, $0, $0      #nop
