global main
%include "menu.inc"
;%include "display.inc"
section .data
    turno db 1
    posicion_soldado dw 0
    posicion_oficial dw 0
    formato_casilla db "Elegiste el oficial de la %hi, y quieres moverlo a la casilla %hi",10, 0
    global matriz
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
section .bss

    casilla resb 2

section .text
main:
    sub rsp, 8
    call menu_juego
    add rsp,8

    mov rdi, formato_casilla
    mov rsi, [posicion_oficial]
    mov rdx, [casilla]
    sub rsp, 8
    call printf
    add rsp, 8

;    sub rsp, 8
;    call imprimir_tablero
;    add rsp,8
    ret
