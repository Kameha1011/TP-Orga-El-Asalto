;Acá irá toda la logica del juego en si, mover piezas
global juego
section .data
    formato_numero db "%d", 0
    formato_prueba db "casilla: %s ,fila: %hi ,columna: %hi",10,0
    txt_casilla_vacia db "La casilla ingresada no está vacía",10,0
    txt_casilla_no_ady db "La casilla ingresada no es adyacente (no puedes moverte mas de 1 casilla)",10,0

    fila_inicial dd 5 ;columna inicial de la fortaleza
    columna_inicial dd 3 ;columna inicial de la fortaleza
    ancho_fortaleza dd 3;ancho de la fortaleza
    largo_fortaleza dd 3 ;Largo de la fortaleza
    total_elementos dd 49 ; cantidad total de elementos en la matriz

    txt_error_ingreso_soldado db "Error en ingreso, la casilla ingresada no contiene un soldado",10,0
    txt_movimiento_soldado_invalido db "El movimiento es invalido. Recuerda que los soldados deben moverse hacia la fortaleza.",10,0
section .bss
    buffer_casilla resw 1

    fila resw 1
    columna resw 1
	fila_partida resw 1
    columna_partida resw 1
    fila_destino resw 1
    columna_destino resw 1

	oficial_1o2 resb 1
	oficial_por_cumplir resb 1
section .text
;------------------------------------------------------------------
;Verificar si la casilla ingresada contiene un soldado
verificar_casilla_soldado:
    mov byte[valido], 1
    obtener_fila_columna casilla, fila, columna
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
    obtener_fila_columna casilla , fila, columna
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
verificar_casilla_vacia:
    mov byte[valido], 1
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
verificar_casilla_eliminar_soldado:
    mov byte[valido], 1

	obtener_fila_columna casilla_destino, fila_destino, columna_destino

    movzx rax, word[fila_destino]   ; inicializo i filas
    movzx rcx, word[columna_destino]   ;inicializo j columnas

    imul rax,7      ;i filas * cant columnas
    add  rax,rcx     ;(i filas * cant columnas) + j columnas
    imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

    movzx rbx, byte[num_soldado]
    movzx rcx, byte[matriz + rax]
    cmp rcx,rbx
    jne bp_error_casilla ;si la casilla no es un soldado -> invalido

    ret ;si llega a este punto la casilla es válida

;------------------------------------------------------------------
error_casilla:
    mov rdi, txt_error_ingreso_soldado
    sub rsp, 8
    call printf
    add rsp, 8
bp_error_casilla:
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
verificar_casilla_destino_adyacente:
    mov byte[valido],0
    sub rsp,8
    call obtener_adyacencias
    add rsp,8
    ;Recorrer matriz adyacencias para comparar cada elem con la casilla destino y ver si pertenece
    mov rcx,0 ; i
    mov rax,0; reiniciar rax
recorrer_adyacencias_filas:
    mov rdx,0
    imul rdx,rcx,2 ; i*tamanoElem
    mov ax,[buffer_casillas_ady + edx]; al = fila, ah = columna
comparar_fila:
    cmp al,[fila_destino]
    je comparar_columna
    jmp continuar_iteracion_adyacentes
comparar_columna:
    cmp ah,[columna_destino]
    jne continuar_iteracion_adyacentes
    mov byte[valido],1
    jmp terminar_verificacion_ady
continuar_iteracion_adyacentes:
    inc rcx
    cmp rcx,8
    jl recorrer_adyacencias_filas
error_casilla_no_ady:
    mov rdi, txt_casilla_no_ady
    sub rsp, 8
    call printf
    add rsp, 8
terminar_verificacion_ady:
    ret
;------------------------------------------------------------------
;Verificar las casillas del fuerte
verificar_fuerte:
    mov byte[soldados_en_fuerte], 0

    call contar_piezas

terminar_verificacion_fuerte:
    ret

contar_piezas:
    mov ecx, 0 ; Contador de filas recorridas
    mov esi, matriz
    mov ebx, [fila_inicial]
    dec ebx ; Ajustar índice de fila inicial a base 0

fila_loop:
    mov eax, ebx ; Copia el número de fila actual
    imul eax, 7 ; Calcula el desplazamiento de la fila (7 columnas por fila)
    add eax, [columna_inicial]
    dec eax ; Ajustar índice de columna inicial a base 0
    mov edi, eax ; Índice inicial de la fila actual

    xor edx, edx ; Contador de columnas recorridas en la fila
columna_loop:
    mov al, [esi + edi]
    cmp al, [num_soldado] ; Comparar valor actual con el valor buscado (88)
    jne siguiente_columna

    ; Incrementar contador si se encuentra un soldado
    inc byte [soldados_en_fuerte]

siguiente_columna:
    inc edi ; Avanzar al siguiente elemento de la fila
    inc edx ; Incrementar contador de columnas locales
    cmp edx, [ancho_fortaleza]
    jl columna_loop ; Continuar recorriendo la fila actual

    ; Pasar a la siguiente fila
    inc ebx
    inc ecx
    cmp ecx, [largo_fortaleza]
    jl fila_loop ; Continuar recorriendo las filas

    ret ; si se termina de iterar la ultima fila salgo de la funcion
;------------------------------------------------------------------
; contar Oficiales y Soldados
verificar_oficiales_y_solados:
    mov byte[cantidad_soldados], 0
    mov byte[cantidad_oficiales], 0
    mov ebx, 0           ; Índice para recorrer la matriz
    mov ecx, [total_elementos] ; ECX tiene el total de elementos a recorrer
    mov esi, matriz      ; ESI apunta al inicio de la matriz

contar_casillas:
    ; Verificamos el valor en la posición actual de la matriz
    mov al, [esi + ebx]
    cmp al, [num_soldado]; Comprobamos si es soldado
    je incrementar_soldados
    cmp al, [num_oficial]; Comprobamos si es oficial
    je incrementar_oficiales

siguiente_elemento:
    ; Avanzamos al siguiente elemento
    inc ebx
    cmp ebx, ecx         ; Si ebx alcanza el total de elementos, terminamos el bucle
    jl contar_casillas

fin_conteo_oficiales_y_soldados:
    ret

incrementar_soldados:
    inc byte [cantidad_soldados]
    jmp siguiente_elemento

incrementar_oficiales:
    inc byte [cantidad_oficiales]
    jmp siguiente_elemento
;------------------------------------------------------------------------------------
verificar_que_oficial_soy:
    mov bx, [posicion_oficial1]
	mov cx, [casilla_partida]
    cmp bx, cx
	je soy_oficial1
	mov byte[oficial_1o2], 0 ;caso oficial 2
    ret
  soy_oficial1:
	mov byte[oficial_1o2], 1
    ret
