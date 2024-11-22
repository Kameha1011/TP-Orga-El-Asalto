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
    call imprimir_tablero
    add rsp,8

    sub rsp, 8
    call empezar_juego
    add rsp,8

    cmp rax,[sys_call]
    je fin
    cmp byte[soldados_en_fuerte],9
    je fin_fuerte
    cmp byte[cantidad_soldados],8
    je fin_soldado
    cmp byte[cantidad_oficiales],0
    je fin_oficial
    jmp main

    ret
fin:
    mov rax,[sys_call]
    mov rdi,[exit_code]
    syscall
fin_oficial:
    sub rsp, 8
    call imprimir_tablero
    add rsp,8
    mov rdi, oficiales_insuficientes
    sub rsp, 8
    call printf
    add rsp, 8

    mov rax,[sys_call]
    mov rdi,[exit_code]
    syscall
fin_soldado:
    sub rsp, 8
    call imprimir_tablero
    add rsp,8
    mov rdi, soldados_insuficientes
    sub rsp, 8
    call printf
    add rsp, 8

    mov rax,[sys_call]
    mov rdi,[exit_code]
    syscall
fin_fuerte:
    sub rsp, 8
    call imprimir_tablero
    add rsp,8
    mov rdi, fuerte_tomado
    sub rsp, 8
    call printf
    add rsp, 8

    mov rax,[sys_call]
    mov rdi,[exit_code]
    syscall
