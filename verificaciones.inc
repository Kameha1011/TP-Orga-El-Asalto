;Acá irá toda la logica del juego en si, mover piezas

global juego
section .data
    formato_numero db "%d", 0
    formato_prueba db "casilla: %s ,fila: %hi ,columna: %hi",10,0
    txt_casilla_vacia db "La casilla ingresada no está vacía",10,0
section .bss
    buffer_casilla resw 1
    fila resw 1
    columna resw 1
section .text
juego:

    ret
;------------------------------------------------------------------
;Verificar si la casilla ingresada contiene un soldado
verificar_casilla_soldado:
    mov byte[valido], 1
    mov rdi, casilla
    sub rsp, 8
    call obtener_fila_columna
    add rsp, 8
    sub rsp, 8
    call verificar_casilla_matriz
    add rsp, 8
    cmp byte[valido], 0
    je salida

    movzx rax, word[fila]   ; inicializo i filas
    movzx rcx, word[columna]   ;inicializo j columnas

    imul rax,7      ;i filas * cant columnas
    add  rax,rcx     ;(i filas * cant columnas) + j columnas
    imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

    movzx rbx, byte[num_soldado]
    movzx rcx, byte[matriz + rax]
    cmp rcx,rbx
    jne error_casilla ;si la casilla no es un soldado -> invalido

    ret ;si llega a este punto la casilla es válida

;------------------------------------------------------------------
;Verificar si la casilla ingresada contiene un oficial
verificar_casilla_oficial:
    mov byte[valido], 1
    mov rdi, casilla
    sub rsp, 8
    call obtener_fila_columna
    add rsp, 8
    sub rsp, 8
    call verificar_casilla_matriz
    add rsp, 8
    cmp byte[valido], 0
    je salida

    movzx rax, word[fila]   ; inicializo i filas
    movzx rcx, word[columna]   ;inicializo j columnas

    imul rax,7      ;i filas * cant columnas
    add  rax,rcx     ;(i filas * cant columnas) + j columnas
    imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

    movzx rbx, byte[num_oficial]
    movzx rcx, byte[matriz + rax]
    cmp rcx,rbx
    jne error_casilla ;si la casilla no es un oficial -> invalido

    ret ;si llega a este punto la casilla es válida

;------------------------------------------------------------------
verificar_casilla_matriz: ;si la casilla pertenece a la matriz
    cmp word[fila], 0
    jl error_casilla
    cmp word[fila], 6
    jg error_casilla
    cmp word[fila], 2 ;si la fila esta entre 3 y 5 entonces verifica que junto a las columnas es una posicion valida
    jl casilla_col_invalida
    cmp word[fila], 4
    jg casilla_col_invalida
    cmp word[columna], 0
    jl error_casilla
    cmp word[columna], 6
    jg error_casilla

    ret
casilla_col_invalida:
    cmp word[columna], 2
    jl error_casilla
    cmp word[columna], 4
    jg error_casilla

    ret
;------------------------------------------------------------------
;ingresar la casilla en rdi
obtener_fila_columna:

    ; Obtener casilla a formato entero
    mov rsi, formato_numero
    mov rdx, buffer_casilla
    sub rsp, 8
    call sscanf
    add rsp, 8

    mov ax, word[buffer_casilla]    ; Cargar el número completo (ej. 23) en AX
    mov bx, 10                ; Divisor (base 10)
    div bx                    ; Divide AX entre BX: AL = AX / BX (cociente), AH = AX % BX (residuo)

    ; Guardar fila y columna
    mov [fila], ax            ; Fila (cociente) en AX
    mov [columna], dx         ; Columna (residuo) en DX

    mov word[buffer_casilla], 0   ; Limpiar buffer

    ret

;------------------------------------------------------------------
;ingresar la casilla en rdi
verificar_casilla_vacia:
    mov byte[valido], 1
    sub rsp, 8
    call obtener_fila_columna
    add rsp, 8
    sub rsp, 8
    call verificar_casilla_matriz
    add rsp, 8
    cmp byte[valido], 0
    je salida

    movzx rax, word[fila]   ; inicializo i filas
    movzx rcx, word[columna]   ;inicializo j columnas

    imul rax,7      ;i filas * cant columnas
    add  rax,rcx     ;(i filas * cant columnas) + j columnas
    imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

    movzx rbx, byte[vacio]
    movzx rcx, byte[matriz + rax]
    cmp rcx,rbx
    jne error_casilla_vacia ;si la casilla no esta vacia -> invalido

    ret ;si llega a este punto la casilla es válida

;------------------------------------------------------------------
error_casilla:
    mov byte[valido], 0
    ret

;------------------------------------------------------------------
error_casilla_vacia:
    mov byte[valido], 0
    mov rdi, txt_casilla_vacia
    sub rsp, 8
    call printf
    add rsp, 8
    ret

;------------------------------------------------------------------
