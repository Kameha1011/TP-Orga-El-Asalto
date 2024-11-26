global movimientos

%macro obtener_fila_columna 3
   	mov al, byte[%1]                ; Dirección de la casilla (entrada)
	mov ah, byte[%1+1]

    sub al, '0' ; Convertir de ASCII a número
    sub ah, '0' ; Convertir de ASCII a número
    ; Guardar fila y columna
    mov byte[%2], al             ; Fila (cociente) en AX
    mov byte[%3], ah             ; Columna (residuo) en DX

%endmacro

section .data
    tiene_que_comer db 1
section .bss


    buffer_casillas_ady resw 8
    buffer_casilla_ady resw 1


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

       obtener_fila_columna casilla_destino, fila_destino, columna_destino

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

        obtener_fila_columna casilla_destino, fila_destino, columna_destino

      ;verificar condiciones
        sub rsp,8
		call verificar_casilla_eliminar_soldado
		add rsp,8
        cmp byte[valido],1
        je comer_soldado

    solo_mover_oficial:
        mov ax, [fila_destino]
        mov [fila], ax
        mov ax, [columna_destino]
        mov [columna], ax

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

        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

        cmp byte[tiene_que_comer], 1
        je eliminar_oficial

actualizar_posicion_oficial:
	;ACTUALIZO LA POSICION DEL OFICIAL ELEGIDO
        mov bl, [casilla_destino]
        ;chequear si mi casilla_partida pertenece a mi oficial1 o a mi oficial2
        sub rsp,8
        call verificar_que_oficial_soy
        add rsp,8
        cmp byte[oficial_1o2], 1
        je oficial1
        mov byte[posicion_oficial2], bl
        jmp cambio_turno_oficial
        oficial1:
        mov byte[posicion_oficial1], bl

        jmp cambio_turno_oficial


    ;------------------------------------------------------------------
    comer_soldado:
        cmp byte[tiene_que_comer], 0
        je solo_mover_oficial

        obtener_fila_columna casilla_partida, fila_partida, columna_partida

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

        movzx rax, word[fila_destino]   ; calculo i filas
        movzx rcx, word[columna_destino]   ;calculo j columnas

        imul rax, rax, 2
        sub ax, [fila_partida]
        mov [fila_destino], rax

        imul rcx, rcx, 2
        sub cx, [columna_partida]
        mov [columna_destino], rcx

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

        dec byte[cantidad_soldados]

    cambio_turno_oficial:
        mov byte[turno], 0
		mov byte[tiene_que_comer], 0
        ret
;------------------------------------------------------------------SI EL OFICIAL NO CUMPLE CON SU DEBER

   eliminar_oficial:
        cmp byte[soldado_por_cumplir], 0
        je eliminar_oficial1
        cmp byte[soldado_por_cumplir], 1
        je eliminar_oficial2
        cmp byte[soldado_por_cumplir], 2
        je eliminar_oficiales

   eliminar_oficial1:
        obtener_fila_columna posicion_oficial1, fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]
      	jmp actualizar_posicion_oficial
   eliminar_oficial2:
        obtener_fila_columna posicion_oficial2, fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]
      	jmp actualizar_posicion_oficial
   eliminar_oficiales:
        obtener_fila_columna posicion_oficial1, fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]

        obtener_fila_columna posicion_oficial2, fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]
      	jmp cambio_turno_oficial




;------------------------------------------------------------------

    obtener_adyacencias:
        cmp word[fila],1
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
