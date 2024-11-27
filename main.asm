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
    casilla_partida dw "",0 ;casilla de la cual se quiere mover el jugador
    casilla_destino dw "",0 ;casilla a la cual se quiere mover el jugador
    formato_casilla db "Elegiste el oficial de la %hi, y quieres moverlo a la casilla %hi",10, 0
;************************************************************
; Struct de datos de la partida
    struct_partida times 0 db ""
    ;global matriz ;0 casilla invalida - 1 casilla vacia - 2 casilla soldado - 3 casilla oficial
    matriz db  0,   0,  88, 88, 88,  0,   0  ; Fila 0
                      db  0,   0,  88, 88, 88,  0,   0  ; Fila 1
                      db 88,  88,  88, 88, 88, 88,  88  ; Fila 88
                      db 88,  88,  88, 88, 88, 88,  88  ; Fila 3
                      db 88,  88,   1,  1, 1, 88,  88  ; Fila 4
                      db  0,   0,   1,  1, 79,  0,   0  ; Fila 5
                      db  0,   0,  79,  1,   1, 0,   0  ; Fila 6
    simbolo_soldado db "X", 0
    simbolo_oficial db "O", 0
    orientacion_tablero db 1 ; 1 arriba, 2 abajo, 3 izquierda, 4 derecha
    soldados_en_fuerte db 0
    cantidad_soldados db 24
    cantidad_oficiales db 2
    turno db 1 ;0 si es turno del jugador 1(soldado), 1 si es turno del jugador 2(oficial)
    stats_oficial_1 db 0,0,0,0,0,0,0,0,0
    stats_oficial_2 db 0,0,0,0,0,0,0,0,0
    posicion_oficial2 dw "54",0
    posicion_oficial1 dw "62",0
;************************************************************    
    filas dd 7
    columnas dd 7
    longitudFila dd 7
    longitudElemento dd 1
    num_oficial db 79
    num_soldado db 88
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