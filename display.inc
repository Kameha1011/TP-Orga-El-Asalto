; Todo lo relacionado a mostrar datos por pantalla
extern matriz
section .data
    soldado db " X │",0
    oficial db " O │",0
    casilla_vacia db "   │",0
    separador db "│",0
    salto_de_linea db 10, 0 ; 10 es el valor ASCII de salto de línea
    indices_columnas db "   0   1   2   3   4   5   6",10,0
    indice_fila db "%i",0
    espacio_inicial db "        │",0
    separador_horizontal_inicial db "         ┌───┬───┬───┐        ",10,0
    separador_horizontal_corto   db "         ├───┼───┼───┤        ",10,0
    separador_horizontal_1       db " ┌───┬───┼───┼───┼───┼───┬───┐",10,0
    separador_horizontal         db " ├───┼───┼───┼───┼───┼───┼───┤",10,0
    separador_horizontal_2       db " └───┴───┼───┼───┼───┼───┴───┘",10,0
    separador_horizontal_final   db "         └───┴───┴───┘        ",10,0
section .bss
section .text
global imprimir_tablero

imprimir_tablero:
    mov r12d, 0 ; i
    sub rsp,8
    call imprimir_indices_columnas
    add rsp,8
    sub rsp,8
    call imprimir_separador_horizontal_inicial
    add rsp,8
recorrer_filas:
    ;inicializamos (i-1)*longitudFila
    mov r13d, [longitudFila] ; Longitud de la fila
    imul r13d, r12d; i * longitudFila
    mov r14d, 0 ; j
    sub rsp,8
    call imprimir_indice_fila
    add rsp,8
    ;Las siguientes dos comparaciones son para verificar si se debe imprimir el "espacio inicial" para dar forma al tablero
    cmp r12d,1
    jle imprimir_espacio_inicial
    cmp r12d,5
    jge imprimir_espacio_inicial
    ;Si no pasó las dos comparaciones anteriores, simplemente imprime un separador que seria la "pared inicial" de cada fila que no empieza con espacios
    sub rsp,8
    call imprimir_separador
    add rsp,8
    jmp recorrer_columnas
imprimir_espacio_inicial:
    mov rdi,espacio_inicial
    sub rsp,8
    call printf
    add rsp,8
recorrer_columnas:
    ;inicializamos (j-1)*longitudElemento 
    mov r15d, [longitudElemento] ; Longitud de un elemento
    imul r15d, r14d; j * longitudElemento
    sub rsp,8
    call imprimir_casilla
    add rsp,8
    ;Incremento j y si j<columnas contiuo iterando por columnas
    inc r14d
    cmp r14d, [columnas]
    jl recorrer_columnas
    sub rsp, 8
    call imprimir_salto_de_linea
    add rsp,8
    ; En las siguientes comparaciones chequeo que en que fila estoy para imprimir el correspondiente "separador horizontal"
    cmp r12d, 0 ; if i == 0
    je imprimir_separador_corto ; print ├───┼───┼───┤
    cmp r12d, 5 ; if i == 5
    je imprimir_separador_corto ; print ├───┼───┼───┤
    cmp r12d, 1 ; if i == 1
    je imprimir_separador_horizontal_1 ; print ┌───┬───┼───┼───┼───┼───┬───┐
    cmp r12d, 4 ; if i == 4
    je imprimir_separador_horizontal_2 ; print └───┴───┼───┼───┼───┼───┴───┘
    cmp r12d, 6 ; if i == 6
    je imprimir_separador_horizontal_final; print └───┴───┴───┘ 
imprimir_separador_horizontal:
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_horizontal_1:
    mov rdi, separador_horizontal_1
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_horizontal_2:
    mov rdi, separador_horizontal_2
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_corto:
    mov rdi, separador_horizontal_corto
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_horizontal_final:
    mov rdi, separador_horizontal_final
    sub rsp, 8
    call printf
    add rsp, 8
continuar_iteracion:
    ;Incremento i y si i<filas contiuo iterando por filas
    inc r12d
    cmp r12d, [filas]
    jl recorrer_filas
    ret

;*** Subrutinas ***

imprimir_salto_de_linea:
    mov rdi, salto_de_linea
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla:
    cmp byte[matriz + r13 + r15], 1 ; si matriz[i][j] > 1 -> hay alguna pieza
    jg imprimir_casilla_ocupada
    cmp byte[matriz + r13 + r15], 1 ; si matriz[i][j] == 1 -> es casilla vacia
    je imprimir_casilla_vacia
    ret

imprimir_casilla_vacia:
    mov rdi, casilla_vacia
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla_ocupada:
    cmp byte[matriz + r13 + r15], 88
    je imprimir_casilla_soldado
    mov rdi, oficial
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla_soldado:
    mov rdi, soldado
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

imprimir_separador: 
    mov rdi, separador
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_indices_columnas:
    mov rdi, indices_columnas
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_indice_fila:
    mov esi, r12d
    mov rdi, indice_fila
    sub rsp, 8
    call printf
    add rsp, 8
    ret
