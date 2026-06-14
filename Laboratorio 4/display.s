#include <avr/io.h>

.section .text

tabla_segmentos:
    .byte 0b00000011    ; 0
    .byte 0b10011111    ; 1
    .byte 0b00100101    ; 2
    .byte 0b00001101    ; 3
    .byte 0b10011001    ; 4
    .byte 0b01001001    ; 5
    .byte 0b01000001    ; 6
    .byte 0b00011111    ; 7
    .byte 0b00000001    ; 8
    .byte 0b00001001    ; 9

.global mostrar_voltaje
mostrar_voltaje:
    push r16
    push r17
    push r18
    push r28
    push r29
    push r30
    push r31

    ; Configurar puertos
    ldi r16, 0b00111101
    out _SFR_IO_ADDR(DDRB), r16
    ldi r16, 0b10010000
    out _SFR_IO_ADDR(DDRD), r16

    ; centivoltios llega en r25:r24
    ; CALCULAR CENTENAS
    clr r28
    ldi r17, 100
cent_loop:
    cpi r25, 0
    brne cent_sub
    cp  r24, r17
    brlo cent_done
cent_sub:
    sub r24, r17
    sbci r25, 0
    inc r28
    rjmp cent_loop
cent_done:

    ; CALCULAR DECENAS
    clr r29
    ldi r17, 10
dec_loop:
    cp  r24, r17
    brlo dec_done
    sub r24, r17
    inc r29
    rjmp dec_loop
dec_done:
    ; r28=centenas, r29=decenas, r24=unidades

    ; Mostrar centenas (izquierda)
    mov r16, r28
    ldi r17, 0b10000000
    call sacanum

    ; Mostrar decenas con punto decimal
    mov r16, r29
    ldi r17, 0b01000000
    call sacanum_punto

    ; Mostrar unidades
    mov r16, r24
    ldi r17, 0b00100000
    call sacanum

    pop r31
    pop r30
    pop r29
    pop r28
    pop r18
    pop r17
    pop r16
    ret

sacanum:
    ldi r30, lo8(tabla_segmentos)
    ldi r31, hi8(tabla_segmentos)
    add r30, r16
    brcc sacanum_ok
    inc r31
sacanum_ok:
    lpm r16, Z
    call enviar_display
    ret

sacanum_punto:
    ldi r30, lo8(tabla_segmentos)
    ldi r31, hi8(tabla_segmentos)
    add r30, r16
    brcc sacanum_pto_ok
    inc r31
sacanum_pto_ok:
    lpm r16, Z
    andi r16, 0b11111110
    call enviar_display
    ret

enviar_display:
    call dato_serie
    mov r16, r17
    call dato_serie
    sbi _SFR_IO_ADDR(PORTD), 4
    cbi _SFR_IO_ADDR(PORTD), 4
    ret

dato_serie:
    ldi r18, 8
loop_dato1:
    cbi _SFR_IO_ADDR(PORTD), 7
    lsr r16
    brcs loop_dato2
    cbi _SFR_IO_ADDR(PORTB), 0
    rjmp loop_dato3
loop_dato2:
    sbi _SFR_IO_ADDR(PORTB), 0
loop_dato3:
    sbi _SFR_IO_ADDR(PORTD), 7
    dec r18
    brne loop_dato1
    ret