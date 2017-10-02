@@ File Name      : mycode.s
@@ PROJECT		  : SENG8005 Assignment #4
@@ PROGRAMMERS	  : Abdel Alkuor, Nimal Kirshna (Group #2)
@@ FIRST VERSION  : 2017-04-18
@@ Description    : The code of blinking  8 LEDs on stm32f303C
@@					is implemented inside my_Tick function
@@					where one led turns on and the previous
@@					led turn off and so on. The interval 
@@			        between blinking each led is 500 ms
 

    .code   16
    .text                   @@ - Code section (text -> ROM)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global mytest          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   mytest, %function   @@ - symbol type (not req)
@@ Declaration : int mytest(int x)
@@ Uses r0 for param 0
@@   r0: x
mytest:
    push {lr}
    push {r1}
    push {r0-r7}
    ldr  r0, =7
    bl   BSP_LED_Toggle           @@ call BSP function
    pop  {r0-r7}
    ldr  r1, =myTickCount
    ldr  r0, [r1]
    pop  {r1} 
    pop  {pc}
    .size   mytest, .-mytest    @@ - symbol size (not req)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ <function block>															   
    .align  2               @@ - 2^n alignment (n=2)						   
    .syntax unified															
    .global my_Tick          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Tick, %function   @@ - symbol type (not req)
@@ Declaration : void my_Tick( void )
@@ Uses nothing
my_Tick:
    push {lr}
    push {r4-r11}
    ldr  r1,=myTickCount
    ldr  r0, [r1]
	ldr  r3,=ledCounter
	cmp  r0,#0
	beq	 turnOnLed
	cmp  r0,#500
	beq  resetCounter
	b    continue
turnOnLed:						  @@Turn on LED[x] at 0 ms
	push {r0,r1,r3}
	ldr  r0,[r3]
    bl   my_LED_Function         @@ turn off LED[x] after 500 ms
    pop  {r0,r1,r3}
	b    continue
resetCounter:
	mov r0,#0
	str r0,[r1]					  @@ reset myTickCount to zero
	push {r0,r1,r3}
	ldr  r0,[r3]
    bl   my_LED_Function         @@ turn off LED[x] after 500 ms
    pop  {r0,r1,r3}
	ldr r3,=ledCounter
	ldr r3,[r3]
	add r3,r3,#1
	ldr r4,=ledCounter
	str r3,[r4]					  @@ increment Led index
	cmp r3,#7
	bgt resetLED				  @@ branch to resetLED when led index is >7
	b	skip
resetLED:
	mov r3,#0				
	ldr r4,=ledCounter
	str r3,[r4]					  @@ reset led index to 0
	b skip
continue:	
	add  r0, r0, #1				  @@ increment myTickCount by 1
    str  r0, [r1]
skip:
    pop  {r4-r11}
    pop  {pc}
    .size   my_Tick, .-my_Tick    @@ - symbol size (not req)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_LED_Function          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_LED_Function, %function   @@ - symbol type (not req)
my_LED_Function:      @@ This function replaces BSP_LEDToggle function for toggling on board leds
	push {r4-r12,lr}
	ldr r1,=LEDsAddress
	ldr r1,[r1]
	ldrh r3,[r1] 
	mov r2,#0x1     
	add r0,#8			   @@ add offset to the LED wants to be toggled by 8
	lsl r2,r2,r0		   @@ shift r0 to the left by led place + offset

	eor r3,r3,r2           @@ toggle LED
	strh r3,[r1]		   @@ store xored value into LEDsAddress register	
	pop {r4-r12,lr}
	bx lr	
	
	.size   my_LED_Function, .-my_LED_Function    @@ - symbol size (not req)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_Loop          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Loop, %function   @@ - symbol type (not req)
@@ Declaration : void my_Loop( void )
@@ Uses nothing
my_Loop:
    push {lr}
    pop  {pc}
    .size   my_Loop, .-my_Loop    @@ - symbol size (not req)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global my_Init          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   my_Init, %function   @@ - symbol type (not req)
@@ Declaration : void my_Init( void )
@@ Uses nothing
my_Init:
    push {lr}
    pop  {pc}
    .size   my_Init, .-my_Init    @@ - symbol size (not req)

    .data
    .global myTickCount
myTickCount:
    .word  -1       /* A 32-bit variable named myTickCount */

	.data
	.global ledCounter
ledCounter:
	.word  0

	.data
	.global LEDsAddress
LEDsAddress:
	.word 0x48001014
    .end

