//.include "./m328Pdef.inc"

.global delay_joel;
.section .text                  ; Defines a code section
//start: 
//	ldi r17,0xFF; cargo valor para delay0
//	ldi r18,0xFF; cargo valor para delay1
//	ldi r19,0x05; cargo valor para delay2
delay_joel:
delay0:
	dec r18
	brne delay0; se queda en delay0 hasta que r17 da cero 
	rjmp delay1; salto a delay1
delay1:
	ldi r18,0xFF;cargo r17 denuevo
	dec r19; decremento r18 y vuelbo a delayo hasta que r18 da cero
	brne delay0; con esto hago loop adidado con delay0
	rjmp delay2;
delay2:
	ldi r18,0xFF;
	ldi r19,0xFF;
	dec r20
	brne delay0; con esto hago loop andidado con delay0 y delay1
	ret