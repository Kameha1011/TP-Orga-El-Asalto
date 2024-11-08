global main
section .data
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
%include "display.inc"
section .text
main:
    sub rsp, 8
    call imprimir_tablero
    add rsp,8
    ret
