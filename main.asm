global main
%include "menu.inc"
;%include "display.inc"
section .data
    turno db 1  ;0 si es turno del jugador 1, 1 si es turno del jugador 2
    posicion_soldado dw 0 ;posicion del soldado elegido para el turno
    posicion_oficial dw 0 ;posicion del oficial elegido para el turno
    formato_casilla db "Elegiste el oficial de la %hi, y quieres moverlo a la casilla %hi",10, 0

    global matriz ;0 casilla invalida - 88 casilla soldado - 79 casilla oficial - 1 casilla vacia
    matriz db  0,   0,  88, 88, 88,  0,   0  ; Fila 1
           db  0,   0,  88, 88, 88,  0,   0  ; Fila 2
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 3
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 4
           db 88,  88,   1,  1,  1, 88,  88  ; Fila 5
           db  0,   0,   1,  1, 79,  0,   0 ; Fila 6
           db  0,   0,  79,  1,   1, 0,   0   ; Fila 7ce
    filas dd 7
    columnas dd 7
    longitudFila dd 7
    longitudElemento dd 1
    oficial db 79
    soldado db 88
section .bss
    casilla resw 1 ;casilla a la que se quiere mover el jugador

section .text
main:
;    sub rsp, 8
;    call imprimir_tablero
;    add rsp,8

    sub rsp, 8
    call menu_juego
    add rsp,8

    mov rdi, formato_casilla
    mov rsi, [posicion_oficial]
    mov rdx, [casilla]
    sub rsp, 8
    call printf
    add rsp, 8

    ret
