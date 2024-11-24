global movimientos

section .data
    tiene_que_comer db 1
    ;flags para la distancia entre casillas
    cumple_horizontal db 0
    cumple_vertical db 0
section .bss

    fila_partida resw 1
    columna_partida resw 1
    fila_destino resw 1
    columna_destino resw 1
    buffer_casillas_ady resw 8
    buffer_casilla_ady resw 1

    comio? resw 1

section .text
    movimientos:
        ret

    ;------------------------------------------------------------------
    movimiento_soldado:
        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia

        mov rdi, casilla_destino
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8

        mov dx, [fila]
        mov [fila_destino], dx
        mov dx, [columna]
        mov [columna_destino], dx

        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_soldado]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el soldado

        ;cambio de turno
        mov byte[turno], 1
        ret
    ;------------------------------------------------------------------
    movimiento_oficial:
        ;obtener fila y columna de la casilla de destino
        mov rdi, casilla_destino
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8
        mov dx, [fila]
        mov [fila_destino], dx
        mov dx, [columna]
        mov [columna_destino], dx

    ;verificar condiciones
        sub rsp,8
		call verificar_casilla_eliminar_soldado
		add rsp,8
        cmp byte[valido],1
        je comer_soldado

    solo_mover_oficial:

    	sub rsp,8
		call verificar_casilla_vacia
		add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_oficial

        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia

        cmp byte[tiene_que_comer], 1 ;verifico si tenia que comer, si es asi no muevo el oficial, solo se elimina
        je bp_muere_oficial


        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial
        jmp cambio_turno_oficial

    bp_muere_oficial:
        dec byte[cantidad_oficiales]
    cambio_turno_oficial:
        mov byte[turno], 0
        ret

    ;------------------------------------------------------------------
    comer_soldado:
        cmp byte[tiene_que_comer], 0
        je solo_mover_oficial

        mov rdi, casilla_partida
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8
        mov dx, [fila]
        mov [fila_partida], dx
        mov dx, [columna]
        mov [columna_partida], dx

        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia

eliminar_soldado:
        movzx rax, word[fila_destino]; inicializo i filas
        movzx rcx, word[columna_destino];inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia

        movzx rax, word[fila_destino]   ; inicializo i filas
        imul rax, rax, 2
        sub rax, [fila_partida]

        movzx rcx, word[columna_destino]   ;inicializo j columnas
        imul rcx, rcx, 2
        sub rcx, [columna_partida]

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

        dec byte[cantidad_soldados]
        ;mov byte[comio?], 1 ;mas al pedo que bocina de avion
        ;cambio de turno
        mov byte[turno], 0
        ret
    ;------------------------------------------------------------------

    obtener_adyacencias:
        cmp word[fila_partida],1
        jl ady_medio_izq
        ady_sup:
            mov al,byte[fila_partida]
            dec al
            mov ah,byte[columna_partida]
            mov [buffer_casillas_ady],ax
        cmp word[columna_partida],1
        jl ady_sup_der
        ady_sup_izq:
            mov al,byte[fila_partida]
            dec al
            mov ah,byte[columna_partida]
            dec ah
            mov [buffer_casillas_ady+2],ax
        cmp word[columna_partida],5
        jg ady_medio_izq
        ady_sup_der:
            mov al,byte[fila_partida]
            dec al
            mov ah,byte[columna_partida]
            inc ah
            mov [buffer_casillas_ady+4],ax
        ady_medio_izq:
            mov al,byte[fila_partida]
            mov ah,byte[columna_partida]
            dec ah
            mov [buffer_casillas_ady+6],ax
        ady_medio_der:
            mov al,byte[fila_partida]
            mov ah,byte[columna_partida]
            inc ah
            mov [buffer_casillas_ady+8],ax
        cmp word[fila_partida],5
        jg devolver_ady
        ady_inf:
            mov al,byte[fila_partida]
            inc al
            mov ah,byte[columna_partida]
            mov [buffer_casillas_ady+10],ax
        ady_inf_izq:
            mov al,byte[fila_partida]
            inc al
            mov ah,byte[columna_partida]
            dec ah
            mov [buffer_casillas_ady+12],ax
        ady_inf_der:
            mov al,byte[fila_partida]
            inc al
            mov ah,byte[columna_partida]
            inc ah
            mov [buffer_casillas_ady+14],ax
        devolver_ady:
            ret
    ;------------------------------------------------------------------
    calcular_distancia_entre_casillas: ;no se va a usar
        mov byte[cumple_horizontal], 0
        mov byte[cumple_vertical], 0
        ; Calcular |x1 - x2|
        mov ax, [columna_partida]
        sub ax, [columna_destino]       ; ax = x1 - x2
        cmp ax, 0
        je vertical
        bp_vertical:
        jge no_neg_x        ; Si es positivo, saltar
        neg ax             ; Convertir a positivo
      no_neg_x:
        mov bx, ax        ; Guardar |x1 - x2| en bx

        ; Calcular |y1 - y2|
        mov ax, [fila_partida]
        sub ax, [fila_destino]       ; ax = y1 - y2
        cmp ax, 0
        je horizontal
        bp_horizontal:
        jge no_neg_y        ; Si es positivo, saltar
        neg ax             ; Convertir a positivo
      no_neg_y:
        ; Comprobar si |x1 - x2| == |y1 - y2|
        cmp bx, ax

      calcular_distancia:
        ; Dividir |x1 - x2| + |y1 - y2| entre 2 y redondear hacia arriba
        add ax, bx        ; ax = |x1 - x2|
        cmp ax, 1        ; Si es 1, no hay que redondear
        je sin_redondeo
        mov bx, 2                ; Divisor (base 2)
        div bx                    ; Divide AX entre BX: AL = AX / BX (cociente), AH = AX % BX (residuo)

        cmp dx,0         ; (residuo) en DX
        je guardar_resultado     ; Si no hay residuo, almacenar resultado
        inc ax             ; Si hay residuo, redondear hacia arriba

      guardar_resultado:
        movzx rax, ax   ; Guardar el resultado en rax : 0 si es un movimiento simple, 1 si es un movimiento doble
        ret
      sin_redondeo:
        dec ax
        movzx rax, ax   ; Guardar el resultado en rax : 0 si es un movimiento simple, 1 si es un movimiento doble
        ret
      no_diagonal:
        cmp byte[cumple_horizontal], 1
        je calcular_distancia
        cmp byte[cumple_vertical], 1
        je calcular_distancia
      invalido:
        movzx rax ,byte[3] ; 3 si no es un movimiento diagonal, horizontal, ni vertical válido
        ret
      horizontal:
        mov byte[cumple_horizontal], 1
        jmp bp_horizontal
      vertical:
        mov byte[cumple_vertical], 1
        jmp bp_vertical