global main
extern printf
extern puts
section .data
    ; Definimos la matriz 7x7
    ; 0 = casillas nulas (no se pintan en pantalla)
    ; 1 = casillas vacias
    ; 'X' = soldados
    ; 'O' = oficiales
    ; casillas nulas a11 a12 a21 a22 a16 a17 a26 a27 a61 a62 a71 a72 a66 a67 a76 a77
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
    call imprimir_tablero
    ret
