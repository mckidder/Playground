
@ ---------------------------------------
@       Data Section
@ ---------------------------------------
        
         .data
         .balign 4      
Intro:   .asciz  "Raspberry Pi wiringPi blink test\n"
ErrMsg:  .asciz "Setup didn't work... Aborting...\n"
pwmpin:  .int   18
pwmhig:  .int   1023
pwmlow:  .int   0
ledpin1:  .int   23
ledpin2:  .int   24
ledpin3:  .int   25
delayMs: .int   75
OUTPUT       =      1
PWM_OUTPUT   =      2
        
@ ---------------------------------------
@       Code Section
@ ---------------------------------------
        
        .text
        .global main
        .extern printf
        .extern wiringPiSetupGpio
        .extern delay
        .extern digitalWrite
        .extern pinMode
        .extern pwmWrite
        
main:   push    {ip, lr}        @ push return address + dummy register
                                @ for alignment

@  printf( "blink..." ) ;
        ldr     r0, =Intro      
        bl      printf          

@  if (wiringPiSetupGpio() == -1) {
@     printf( "Setup didn't work... Aborting." ) ;
@     exit (1)                                   ;
@  }
        bl      wiringPiSetupGpio
        mov     r1,#-1
        cmp     r0, r1
        bne     init
        ldr     r0, =ErrMsg
        bl      printf
        b       done
        
@  pinMode(ledpin1, OUTPUT)         ;
init:
        ldr     r0, =ledpin1
        ldr     r0, [r0]
        mov     r1, #OUTPUT
        bl      pinMode
        
@  pinMode(ledpin2, OUTPUT)         ;

        ldr     r0, =ledpin2
        ldr     r0, [r0]
        mov     r1, #OUTPUT
        bl      pinMode
        
@  pinMode(ledpin3, OUTPUT)         ;

        ldr     r0, =ledpin3
        ldr     r0, [r0]
        mov     r1, #OUTPUT
        bl      pinMode
        
@  pinMode(pwmpin, PWM_OUTPUT)         ;

        ldr     r0, =pwmpin
        ldr     r0, [r0]
        mov     r1, #PWM_OUTPUT
        bl      pinMode

@   while(1){

whileLoop:
		          
        
@       pwmWrite(pwmPin, pwmValue);
        ldr     r0, =pwmpin
        ldr     r0, [r0]
        ldr     r1, =pwmlow
        ldr     r1, [r1]
        bl      pwmWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay

@       digitalWrite(pin1, 1)    ;
        ldr     r0, =ledpin1
        ldr     r0, [r0]
        mov     r1, #1
        bl      digitalWrite

@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay
        
@       digitalWrite(pin1, 0)    ;
        ldr     r0, =ledpin1
        ldr     r0, [r0]
        mov     r1, #0
        bl      digitalWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay

@       digitalWrite(pin2, 1)    ;
        ldr     r0, =ledpin2
        ldr     r0, [r0]
        mov     r1, #1
        bl      digitalWrite

@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay
        
@       digitalWrite(pin2, 0)    ;
        ldr     r0, =ledpin2
        ldr     r0, [r0]
        mov     r1, #0
        bl      digitalWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay

@       digitalWrite(pin3, 1)    ;
        ldr     r0, =ledpin3
        ldr     r0, [r0]
        mov     r1, #1
        bl      digitalWrite

@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay
        
@       digitalWrite(pin3, 0)    ;
        ldr     r0, =ledpin3
        ldr     r0, [r0]
        mov     r1, #0
        bl      digitalWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay
        
@       pwmWrite(pwmPin, pwmValue);
        ldr     r0, =pwmpin
        ldr     r0, [r0]
        ldr     r1, =pwmhig
        ldr     r1, [r1]
        bl      pwmWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay

        b       whileLoop
        
done:   
        pop     {ip, pc}        @ pop return address into pc
