; Todo lo relacionado a mostrar datos por pantalla
;extern matriz
section .data
    simbolo_soldado db "X",0
    simbolo_oficial db "O",0
    soldado db " %s │",0
    oficial db " %s │",0
    casilla_vacia db "   │",0
    separador db "│",0
    espacio_en_blanco db " ",0
    salto_de_linea db 10, 0 ; 10 es el valor ASCII de salto de línea
    indices_columnas db "   0   1   2   3   4   5   6",10,0
    indices_columnas_180 db "   6   5   4   3   2   1   0",10,0
    orientacion_4 db "Orientación:",10,"Filas: ↓,  Cols: ←",10,0
    orientacion_2 db "Orientación:",10,"Filas: ←,  Cols: ↓",10,0
    orientacion_3 db "Orientación:",10,"Filas: ↑,  Cols: →",10,0
    orientacion_1 db "Orientación:",10,"Filas: →,  Cols: ↓",10,0
    indice_fila db "%i",0
    espacio_inicial db "        │",0
    espacio_final db "        ",0
    separador_horizontal_inicial db "         ┌───┬───┬───┐        ",10,0
    separador_horizontal_corto   db "         ├───┼───┼───┤        ",10,0
    separador_horizontal_abajo       db " ┌───┬───┼───┼───┼───┼───┬───┐",10,0
    separador_horizontal         db " ├───┼───┼───┼───┼───┼───┼───┤",10,0
    separador_horizontal_arriba       db " └───┴───┼───┼───┼───┼───┴───┘",10,0
    separador_horizontal_final   db "         └───┴───┴───┘        ",10,0
section .bss
section .text
global imprimir_tablero_rotado_arriba
; rotar_arriba(180g respecto del fuerte) indices columnas -> Abajo, indices filas -> Derecha (imprimir desde ultima fila de derecha a izq)
; rotar_izquierda(-90g respecto del fuerte) indices columnas -> derecha, indices filas -> Arriba (imprimir por cols desde la primera)
; rotar_derecha(90g respecto del fuerte) indices columnas -> Izquierda, indices filas -> abajo (imprimir por cols desde la ultima)



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
    je imprimir_separador_horizontal_abajo ; print ┌───┬───┼───┼───┼───┼───┬───┐
    cmp r12d, 4 ; if i == 4
    je imprimir_separador_horizontal_arriba ; print └───┴───┼───┼───┼───┼───┴───┘
    cmp r12d, 6 ; if i == 6
    je imprimir_separador_horizontal_final; print └───┴───┴───┘ 
imprimir_separador_horizontal:
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_horizontal_abajo:
    mov rdi, separador_horizontal_abajo
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion
imprimir_separador_horizontal_arriba:
    mov rdi, separador_horizontal_arriba
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
    mov rdi, orientacion_1
    sub rsp,8
    call printf
    add rsp,8
    ret

;(rotacion 180)
imprimir_tablero_rotado_arriba:
    mov r12d, 6 ; i
    mov rdi, indices_columnas_180
    sub rsp,8
    call printf
    add rsp,8
    sub rsp,8
    call imprimir_separador_horizontal_inicial
    add rsp,8
recorrer_filas_2:
;inicializamos (i-1)*longitudFila
    mov r13d, [longitudFila] ; Longitud de la fila
    imul r13d, r12d; i * longitudFila
    mov r14d, 6 ; j
    sub rsp,8
    call imprimir_espacio_en_blanco
    add rsp,8
    ;Las siguientes dos comparaciones son para verificar si se debe imprimir el "espacio inicial" para dar forma al tablero
    cmp r12d,1
    jle imprimir_espacio_inicial_2
    cmp r12d,5
    jge imprimir_espacio_inicial_2
    ;Si no pasó las dos comparaciones anteriores, simplemente imprime un separador que seria la "pared inicial" de cada fila que no empieza con espacios
    sub rsp,8
    call imprimir_separador
    add rsp,8
    jmp recorrer_columnas_2
imprimir_espacio_inicial_2:
    mov rdi,espacio_inicial
    sub rsp,8
    call printf
    add rsp,8
recorrer_columnas_2:
    ;inicializamos (j-1)*longitudElemento 
    mov r15d, [longitudElemento] ; Longitud de un elemento
    imul r15d, r14d; j * longitudElemento
    sub rsp,8
    call imprimir_casilla
    add rsp,8
    ;Incremento j y si j<columnas contiuo iterando por columnas
    dec r14d
    cmp r14d, 0
    jge recorrer_columnas_2
    cmp r12d,1
    jle imprimir_espacio_final
    cmp r12d,5
    jge imprimir_espacio_final
    jmp imprimir_indice_fila_2
imprimir_espacio_final:
    mov rdi, espacio_final
    sub rsp,8
    call printf
    add rsp,8
imprimir_indice_fila_2:
    sub rsp,8
    call imprimir_indice_fila
    add rsp,8
    sub rsp, 8
    call imprimir_salto_de_linea
    add rsp,8
    ; En las siguientes comparaciones chequeo que en que fila estoy para imprimir el correspondiente "separador horizontal"
    cmp r12d, 6 ; if i == 6
    je imprimir_separador_corto_2 ; print ├───┼───┼───┤
    cmp r12d, 1 ; if i == 1
    je imprimir_separador_corto_2 ; print ├───┼───┼───┤
    cmp r12d, 5 ; if i == 5
    je imprimir_separador_horizontal_abajo_2 ; print ┌───┬───┼───┼───┼───┼───┬───┐
    cmp r12d, 2 ; if i == 2
    je imprimir_separador_horizontal_arriba_2 ; print └───┴───┼───┼───┼───┼───┴───┘
    cmp r12d, 0 ; if i == 0
    je imprimir_separador_horizontal_final_2; print └───┴───┴───┘ 
imprimir_separador_horizontal_2:
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_2
imprimir_separador_horizontal_abajo_2:
    mov rdi, separador_horizontal_abajo
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_2
imprimir_separador_horizontal_arriba_2:
    mov rdi, separador_horizontal_arriba
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_2
imprimir_separador_corto_2:
    mov rdi, separador_horizontal_corto
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_2
imprimir_separador_horizontal_final_2:
    mov rdi, separador_horizontal_final
    sub rsp, 8
    call printf
    add rsp, 8
continuar_iteracion_2:
    ;Incremento i y si i<filas contiuo iterando por filas
    dec r12d
    cmp r12d, 0
    jge recorrer_filas_2
    mov rdi, orientacion_2
    sub rsp,8
    call printf
    add rsp,8
    ret

;(rotacion 90)
imprimir_tablero_rotado_der:
    mov r12d, 6 ; j
    sub rsp,8
    call imprimir_separador_horizontal_inicial
    add rsp,8
recorrer_columnas_3:
;inicializamos (j-1)*longitudElemento
    mov r13d, [longitudElemento] ; Longitud de la columna
    imul r13d, r12d; j * longitudElemento
    mov r14d, 0 ; i
    mov rdi,indice_fila
    mov esi, r12d
    sub rsp,8
    call printf
    add rsp,8
    ;Las siguientes dos comparaciones son para verificar si se debe imprimir el "espacio inicial" para dar forma al tablero
    cmp r12d,1
    jle imprimir_espacio_inicial_3
    cmp r12d,5
    jge imprimir_espacio_inicial_3
    ;Si no pasó las dos comparaciones anteriores, simplemente imprime un separador que seria la "pared inicial" de cada fila que no empieza con espacios
    sub rsp,8
    call imprimir_separador
    add rsp,8
    jmp recorrer_filas_3
imprimir_espacio_inicial_3:
    mov rdi,espacio_inicial
    sub rsp,8
    call printf
    add rsp,8
recorrer_filas_3:
    ;inicializamos (j-1)*longitudFila 
    mov r15d, [longitudFila] ; Longitud de un elemento
    imul r15d, r14d; i * longitudFila
    sub rsp,8
    call imprimir_casilla
    add rsp,8
    ;Incremento j y si j<columnas contiuo iterando por columnas
    inc r14d
    cmp r14d, 7
    jl recorrer_filas_3
    cmp r12d,1
    jle imprimir_espacio_final_2
    cmp r12d,5
    jge imprimir_espacio_final_2
    jmp continuar_a_salto
imprimir_espacio_final_2:
    mov rdi, espacio_final
    sub rsp,8
    call printf
    add rsp,8
continuar_a_salto:
    sub rsp, 8
    call imprimir_salto_de_linea
    add rsp,8
    ; En las siguientes comparaciones chequeo que en que fila estoy para imprimir el correspondiente "separador horizontal"
    cmp r12d, 6 ; if i == 6
    je imprimir_separador_corto_3 ; print ├───┼───┼───┤
    cmp r12d, 1 ; if i == 1
    je imprimir_separador_corto_3 ; print ├───┼───┼───┤
    cmp r12d, 5 ; if i == 5
    je imprimir_separador_horizontal_abajo_3 ; print ┌───┬───┼───┼───┼───┼───┬───┐
    cmp r12d, 2 ; if i == 2
    je imprimir_separador_horizontal_arriba_3 ; print └───┴───┼───┼───┼───┼───┴───┘
    cmp r12d, 0 ; if i == 0
    je imprimir_separador_horizontal_final_3; print └───┴───┴───┘ 
imprimir_separador_horizontal_3:
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_3
imprimir_separador_horizontal_abajo_3:
    mov rdi, separador_horizontal_abajo
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_3
imprimir_separador_horizontal_arriba_3:
    mov rdi, separador_horizontal_arriba
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_3
imprimir_separador_corto_3:
    mov rdi, separador_horizontal_corto
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_3
imprimir_separador_horizontal_final_3:
    mov rdi, separador_horizontal_final
    sub rsp, 8
    call printf
    add rsp, 8
continuar_iteracion_3:
    ;Incremento i y si i<filas contiuo iterando por filas
    dec r12d
    cmp r12d, 0
    jge recorrer_columnas_3
    mov rdi, indices_columnas
    sub rsp,8
    call printf
    add rsp,8
    mov rdi, orientacion_3
    sub rsp,8
    call printf
    add rsp,8
    ret

;(rotacion -90)
imprimir_tablero_rotado_izq:
    mov r12d, 0 ; j
    mov rdi, indices_columnas_180
    sub rsp,8
    call printf
    add rsp,8
    sub rsp,8
    call imprimir_separador_horizontal_inicial
    add rsp,8
recorrer_columnas_4:
;inicializamos (j-1)*longitudElemento
    mov r13d, [longitudElemento] ; Longitud de la columna
    imul r13d, r12d; j * longitudElemento
    mov r14d, 6 ; i
    sub rsp,8
    call imprimir_espacio_en_blanco
    add rsp,8
    ;Las siguientes dos comparaciones son para verificar si se debe imprimir el "espacio inicial" para dar forma al tablero
    cmp r12d,1
    jle imprimir_espacio_inicial_4
    cmp r12d,5
    jge imprimir_espacio_inicial_4
    ;Si no pasó las dos comparaciones anteriores, simplemente imprime un separador que seria la "pared inicial" de cada fila que no empieza con espacios
    sub rsp,8
    call imprimir_separador
    add rsp,8
    jmp recorrer_filas_4
imprimir_espacio_inicial_4:
    mov rdi,espacio_inicial
    sub rsp,8
    call printf
    add rsp,8
recorrer_filas_4:
    ;inicializamos (j-1)*longitudFila 
    mov r15d, [longitudFila] ; Longitud de un elemento
    imul r15d, r14d; i * longitudFila
    sub rsp,8
    call imprimir_casilla
    add rsp,8
    ;Incremento j y si j<columnas contiuo iterando por columnas
    dec r14d
    cmp r14d, 0
    jge recorrer_filas_4
    cmp r12d,1
    jle imprimir_espacio_final_3
    cmp r12d,5
    jge imprimir_espacio_final_3
    jmp continuar_a_salto_2
imprimir_espacio_final_3:
    mov rdi, espacio_final
    sub rsp,8
    call printf
    add rsp,8
continuar_a_salto_2:
    mov rdi,indice_fila
    mov esi, r12d
    sub rsp,8
    call printf
    add rsp,8
    sub rsp, 8
    call imprimir_salto_de_linea
    add rsp,8
    ; En las siguientes comparaciones chequeo que en que fila estoy para imprimir el correspondiente "separador horizontal"
    cmp r12d, 0 ; if i == 0
    je imprimir_separador_corto_4 ; print ├───┼───┼───┤
    cmp r12d, 5 ; if i == 5
    je imprimir_separador_corto_4 ; print ├───┼───┼───┤
    cmp r12d, 1 ; if i == 1
    je imprimir_separador_horizontal_abajo_4 ; print ┌───┬───┼───┼───┼───┼───┬───┐
    cmp r12d, 4 ; if i == 4
    je imprimir_separador_horizontal_arriba_4 ; print └───┴───┼───┼───┼───┼───┴───┘
    cmp r12d, 6 ; if i == 6
    je imprimir_separador_horizontal_final_4; print └───┴───┴───┘ 
imprimir_separador_horizontal_4:
    mov rdi, separador_horizontal
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_4
imprimir_separador_horizontal_abajo_4:
    mov rdi, separador_horizontal_abajo
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_4
imprimir_separador_horizontal_arriba_4:
    mov rdi, separador_horizontal_arriba
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_4
imprimir_separador_corto_4:
    mov rdi, separador_horizontal_corto
    sub rsp, 8
    call printf
    add rsp, 8
    jmp continuar_iteracion_4
imprimir_separador_horizontal_final_4:
    mov rdi, separador_horizontal_final
    sub rsp, 8
    call printf
    add rsp, 8
continuar_iteracion_4:
    ;Incremento i y si i<filas contiuo iterando por filas
    inc r12d
    cmp r12d, 7
    jl recorrer_columnas_4
    mov rdi, orientacion_4
    sub rsp,8
    call printf
    add rsp,8
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
    mov rsi, simbolo_oficial
    sub rsp, 8
    call printf
    add rsp, 8
    ret

imprimir_casilla_soldado:
    mov rdi, soldado
    mov rsi, simbolo_soldado
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

imprimir_espacio_en_blanco:
    mov rdi, espacio_en_blanco
    sub rsp,8
    call printf
    add rsp,8
    ret
