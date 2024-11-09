;Acá irá toda la logica del juego en si, mover piezas

global juego
section .data

section .bss
    fila resb 1
    columna resb 1
section .text
juego:

    ret
;------------------------------------------------------------------
;Verificar si la casilla ingresada contiene un soldado
verificar_casilla_soldado:
    mov byte[valido], 1
    sub rsp, 8
    call obtener_fila_columna
    add rsp, 8
    sub rsp, 8
    call verificar_casilla_matriz
    add rsp, 8
    cmp byte[valido], 0
    je salida

    mov rdi,[fila - 1]   ;inicializo i filas
    mov rbx,[columna - 1]   ;inicializo j columnas

    imul rdi,7      ;i filas * cant columnas
    add  rdi,rsi     ;(i filas * cant columnas) + j columnas
    imul rdi,2       ;((i filas * cant columnas) + j columnas) * long elemento

    mov ax, [soldado]
    cmp [matriz + rdi],ax
    jne error_casilla ;si la casilla no es un soldado -> invalido

    ret ;si llega a este punto la casilla es válida


;Verificar si la casilla ingresada contiene un oficial
verificar_casilla_oficial:
    mov byte[valido], 1
    sub rsp, 8
    call obtener_fila_columna
    add rsp, 8
    sub rsp, 8
    call verificar_casilla_matriz
    add rsp, 8
    cmp byte[valido], 0
    je salida

    mov rdi,[fila - 1]   ;inicializo i filas
    mov rbx,[columna - 1]   ;inicializo j columnas

    imul rdi,7      ;i filas * cant columnas
    add  rdi,rsi     ;(i filas * cant columnas) + j columnas
    imul rdi,2       ;((i filas * cant columnas) + j columnas) * long elemento

    mov ax, [oficial]
    cmp [matriz + rdi],ax
    jne error_casilla ;si la casilla no es un oficial -> invalido

    ret ;si llega a este punto la casilla es válida

;------------------------------------------------------------------
verificar_casilla_matriz: ;si la casilla pertenece a la matriz
    cmp byte[fila], 0
    jl error_casilla
    cmp byte[fila], 7
    jg error_casilla
    cmp byte[fila], 3 ;si la fila esta entre 3 y 5 entonces verifica que junto a las columnas es una posicion valida
    jl casilla_col_invalida
    cmp byte[fila], 5
    jg casilla_col_invalida
    cmp byte[columna], 0
    jl error_casilla
    cmp byte[columna], 7
    jg error_casilla

    ret
casilla_col_invalida:
    cmp byte[columna], 3
    jl error_casilla
    cmp byte[columna], 5
    jg error_casilla

    ret
;------------------------------------------------------------------
obtener_fila_columna:
    ; Obtener fila y columna
    mov ax, word [casilla]    ; Cargar el número completo (ej. 23) en AX
    mov bx, 10                ; Divisor (base 10)
    div bx                    ; Divide AX entre BX: AL = AX / BX (cociente), AH = AX % BX (residuo)
    ;calcular posicion en matriz

    ; Guardar fila y columna
    mov [fila], al            ; Fila (cociente) en AL
    mov [columna], ah         ; Columna (residuo) en AH
    ret
;------------------------------------------------------------------
error_casilla:
    mov byte[valido], 0
    ret
;------------------------------------------------------------------
salida:
    ret