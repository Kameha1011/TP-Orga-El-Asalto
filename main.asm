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
    matriz db  0,   0,  88, 88, 88,  0,   0  ; Fila 1
           db  0,   0,  88, 88, 88,  0,   0  ; Fila 2
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 3
           db 88,  88,  88, 88, 88, 88,  88  ; Fila 4
           db 88,  88,   1,  1,  1, 88,  88  ; Fila 5
           db  0,   0,   1,  1, 79,  0,   0 ; Fila 6
           db  0,   0,  79,  1,   1, 0,   0   ; Fila 7ce
    fondo_rojo db 0x1B, '[', '4', '1', 'm', 0
    fondo_verde db 0x1B, '[', '4', '2', 'm', 0
    fondo_reset db 0x1B, '[', '0', 'm', 0
    fondo_gris db "\033[47m",0
    soldado db " 💂",0
    oficial db " 🥷 ",0
    espacio db "   ",0
    salto_de_linea db 10, 0 ; 10 es el valor ASCII de salto de línea
    casilla_vacia db "   ",0
    indices_columnas db "  1   2   3   4   5   6   7",0
    indice_fila db "%c",0
    separador_horizontal_inicial db "         ┌───┬───┬───┐        ",0
    eparador_horizontal_corto    db "         ├───┼───┼───┤        ",0
    separador_horizontal_1       db " ┌───┬───┼───┼───┼───┼───┬───┐",0
    separador_horizontal         db " ├───┼───┼───┼───┼───┼───┼───┤",0
    separador_horizontal_2       db " └───┴───┼───┼───┼───┼───┴───┘",0
    separador_horizontal_final   db "         └───┴───┴───┘        ",0
    separador  db "│",0
    filas dd 7
    columnas dd 7
    longitudFila dd 7
    longitudElemento dd 1
section .bss
section .text
main:
    call imprimir_tablero
    ret
imprimir_tablero:
    mov r12d, 0 ; i
    call imprimir_indices_columnas
    call imprimir_salto_de_linea
    call imprimir_separador_horizontal
    call imprimir_salto_de_linea
recorrer_filas:
    mov r13d, [longitudFila] ; Longitud de la fila
    imul r13d, r12d; i * longitudFila
    mov r14d, 0 ; j
    call imprimir_indice_fila
    call imprimir_casilla
    cmp byte[matriz + r12 + r14], 0
    je recorrer_columnas
    call imprimir_separador
recorrer_columnas:
    mov r15d, [longitudElemento] ; Longitud de un elemento
    imul r15d, r14d; j * longitudElemento
    call imprimir_casilla
    cmp byte[matriz + r12 + r14], 0
    je saltar_separador
    call imprimir_separador
saltar_separador:
    inc r14d
    cmp r14d, [columnas]
    jl recorrer_columnas
    inc r12d
    call imprimir_salto_de_linea
    call imprimir_separador_horizontal
    call imprimir_salto_de_linea
    cmp r12d, [filas]
    jl recorrer_filas
    ret

imprimir_espacio:
    mov rdi, espacio
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_salto_de_linea:
    mov rdi, salto_de_linea
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla:
    cmp byte[matriz + r13 + r15], 0
    je imprimir_casilla_nula
    cmp byte[matriz + r13 + r15], 1
    je imprimir_casilla_vacia
    call imprimir_casilla_ocupada
    ret

imprimir_casilla_vacia:
    mov rdi, casilla_vacia
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla_nula:
    mov rdi, espacio
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla_ocupada:
    cmp byte[matriz + r13 + r15], 88
    je imprimir_casilla_soldado
    mov rdi, oficial
    sub rsp, 16
    call printf
    add rsp, 16
    ret

imprimir_casilla_soldado:
    mov rdi, soldado
    sub rsp, 16
    call printf
    add rsp, 16
    ret

imprimir_separador_horizontal:
    cmp r12d, 0
    je imprimir_separador_horizontal_inicial
    cmp r12d, [filas]
    je imprimir_separador_horizontal_final
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_separador_horizontal_inicial:
    mov rdi, separador_horizontal_inicial
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_separador_horizontal_final:
    mov rdi, separador_horizontal_final
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_separador: 
    mov rdi, separador
    sub rsp, 16
    call printf
    add rsp, 16
    ret

imprimir_indices_columnas:
    mov rdi, indices_columnas
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_indice_fila:
    movzx rsi, r12b ; Move and zero-extend the lower byte of r12 to rsi
    add rsi, '1'    ; Convert the index to the corresponding ASCII character
    mov rdi, indice_fila
    sub rsp, 8
    call printf
    add rsp, 8
    ret

pintar_fondo:
    cmp r14d, 1; j <= 1
    jle pintar_esquinas_izquierda
    cmp r14d, 5; j >= 5
    jge pintar_esquinas_izquierda
    ret

pintar_esquinas_izquierda:
    cmp r12d, 1 ; i <= 1
    jle pintar_casilla_rojo
    cmp r12d, 5; i >= 5
    jge pintar_casilla_rojo
    ret

pintar_casilla_rojo:
    mov rdi, fondo_rojo
    sub rsp, 8
    call printf
    add rsp, 8
    mov rdi,fondo_reset
    sub rsp, 8
    call printf
    add rsp, 8
    ret