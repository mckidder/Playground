
@ ---------------------------------------
@       Data Section
@ ---------------------------------------
        
         .data
         .balign 4      
Intro:   .asciz  "Raspberry Pi wiringPi blink test\n"
ErrMsg:  .asciz "Setup didn't work... Aborting...\n"
pwmpin:  .int   18
pwmhig:  .int   1023
pwmlow:  .int   256
ledpin:  .int   23
butpin:  .int   17
delayMs: .int   75
INPUT        =      0
OUTPUT       =      1
PWM_OUTPUT   =      2
PUD_UP       =      2
        
@ ---------------------------------------
@       Code Section
@ ---------------------------------------
        
        .text
        .global main
        .extern printf
        .extern wiringPiSetupGpio
        .extern delay
        .extern digitalWrite
        .extern digitalRead
        .extern pinMode
        .extern pullUpDnControl
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
        
@  pinMode(ledpin, OUTPUT)         ;
init:
        ldr     r0, =ledpin
        ldr     r0, [r0]
        mov     r1, #OUTPUT
        bl      pinMode
        
@  pinMode(pwmpin, PWM_OUTPUT)         ;

        ldr     r0, =pwmpin
        ldr     r0, [r0]
        mov     r1, #PWM_OUTPUT
        bl      pinMode
        
@  pinMode(butpin, INPUT)         ;

        ldr     r0, =butpin
        ldr     r0, [r0]
        mov     r1, #INPUT
        bl      pinMode
        
@  pullUpDnControl(butPin, PUD_UP);

        ldr     r0, =butpin
        ldr     r0, [r0]
        mov     r1, #PUD_UP
        bl      pullUpDnControl

@   while(1){

whileLoop:
@       buttonPressed = 1-digitalRead(butPin);
        ldr     r0, =butpin
        ldr     r0, [r0]
        bl      digitalRead
		mov     r4, r0
		cmp     r4, #0
		
@		if (!buttonPressed)		
		beq		pressed
		
@       pwmWrite(pwmPin, pwmValue);
        ldr     r0, =pwmpin
        ldr     r0, [r0]
        ldr     r1, =pwmhig
        ldr     r1, [r1]
        bl      pwmWrite
        
@       digitalWrite(ledPin, LOW);
        ldr     r0, =ledpin
        ldr     r0, [r0]
        mov     r1, #0
        bl      digitalWrite
        
        b		whileLoop 		
		
pressed:		          
        
@       pwmWrite(pwmPin, pwmValue/4);
        ldr     r0, =pwmpin
        ldr     r0, [r0]
        ldr     r1, =pwmlow
        ldr     r1, [r1]
        bl      pwmWrite
        
@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay

@       digitalWrite(pin, 1)    ;
        ldr     r0, =ledpin
        ldr     r0, [r0]
        mov     r1, #1
        bl      digitalWrite

@       delay(75)               ;
        ldr     r0, =delayMs
        ldr     r0, [r0]
        bl      delay
        
@       digitalWrite(pin, 0)    ;
        ldr     r0, =ledpin
        ldr     r0, [r0]
        mov     r1, #0
        bl      digitalWrite

        b       whileLoop
        
done:   
        pop     {ip, pc}        @ pop return address into pc
