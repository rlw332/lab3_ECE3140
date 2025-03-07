/*
Provided by Nils Napp
*/
.section .text

.global set_led
.type set_led %function

.global led_off
.type led_off %function

.global loop_3cycles
.type loop_3cycles %function


//Small timing loop to create predictable delays
.balign 4
loop_3cycles:
	SUB R0,#1
	BNE loop_3cycles
	BX LR

.balign 4
led_off:
	PUSH {LR}
	MOV R0, #0
	BL set_led
	POP {PC}

//Binary code for set_led
.balign 4
set_led:
	.word 0x490eb470
	.word 0x4b0f4a0e
	.word 0x25011c04
	.word 0x1c2e05ed
	.word 0xd1044026
	.word 0x46c06011
	.word 0x601946c0
	.word 0x6011e009
	.word 0x1c2d46c0
	.word 0x46c01c2d
	.word 0x1c3f46c0
	.word 0x46c01c3f
	.word 0x086d6019
	.word 0xd1ea2d00
	.word 0x4770bc70
	.word 0x00000008
	.word 0x400ff104
	.word 0x400ff108
