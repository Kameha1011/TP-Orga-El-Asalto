global main
%include "menu.inc"
%include "display.inc"

extern printf
extern scanf
extern sscanf

section .data
    soldados_insuficientes db "No quedan suficientes soldados, los oficiales ganan la partida",10,0
    oficiales_insuficientes db "No quedan oficiales, los soldados ganan la partida",10,0
    fuerte_tomado db "El fuerte ha sido tomado, los soldados ganan la partida",10,0


    turno db 0  ;0 si es turno del jugador 1(soldado), 1 si es turno del jugador 2(oficial)
    casilla_destino dw "",0 ;casilla a la cual se quiere mover el jugador
    formato_casilla db "Elegiste el oficial de la %hi, y quieres moverlo a la casilla %hi",10, 0

    global matriz ;0 casilla invalida - 88 casilla soldado - 79 casilla oficial - 1 casilla vacia
    matriz db  0,   0,  88, 88, 88,  0,   0  ; Fila 0
           db  0,   0,  88, 88, 88,  0,   0  ; Fila 1
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 2
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 3
           db 88,  88,   1,  1, 1, 88,  88  ; Fila 4
           db  0,   0,   1,  1, 79,  0,   0  ; Fila 5
           db  0,   0,  79,  1,   1, 0,   0  ; Fila 6ce
    filas dd 7
    columnas dd 7
    longitudFila dd 7
    longitudElemento dd 1
    num_oficial db 79
    num_soldado db 88


    soldados_en_fuerte db 0
    cantidad_oficiales db 2
    cantidad_soldados db 24

    vacio db 1
    sys_call dq 60
    exit_code dq 0
section .bss
    casilla resb 2 ;casilla a la que se quiere mover el jugador

section .text
main:

    sub rsp, 8
    call menu_principal
    add rsp, 8

    ret

limpiar_pantalla:
    mov rdi, cmd_clear
    sub rsp, 8
    call system
    add rsp, 8
    ret