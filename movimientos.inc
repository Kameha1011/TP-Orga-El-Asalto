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

    ;auxiliares
    tiene_que_comer_1 db 1
    tiene_que_comer_2 db 1
section .bss
	tiene_que_comer resb 1

    buffer_casillas_ady resw 8
    buffer_casilla_ady resw 1



    buffer_casillas_ady_trad resw 8
    movimientos_optimos resw 8
    optimo resw 1
section .text
    movimientos:
        ret

    ;------------------------------------------------------------------
    movimiento_soldado:
        ;movi para abajo para no marcar la casilla de partida de soldado como vacia antes de validar si el movimiento es valido
        ;movzx rax, word[fila_partida]   ; inicializo i filas
        ;movzx rcx, word[columna_partida]   ;inicializo j columnas

        ;imul rax,7      ;i filas * cant columnas
        ;add  rax,rcx     ;(i filas * cant columnas) + j columnas
        ;imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        ;mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia

       ;obtener_fila_columna casilla_destino, fila_destino, columna_destino

        sub rsp,8
        call verificar_movimiento_soldado_valido
        add rsp,8
        
        cmp byte[valido],0
        jne mover_soldado_a_destino
        je  bifurcar_a_turno
        ret
        mover_soldado_a_destino:
        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia

       ;obtener_fila_columna casilla_destino, fila_destino, columna_destino

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
        mov byte[tiene_que_comer], 0
        mov byte[tiene_que_comer_1], 0
        mov byte[tiene_que_comer_2], 0
        mov byte[oficial_por_cumplir], 0
        ;obtener fila y columna de la casilla de destino

        obtener_fila_columna casilla_destino, fila_destino, columna_destino

      ;verificar condiciones
        sub rsp,8
        call verificar_movimiento_oficial_valido ;tiene_que_comer
        add rsp,8

        sub rsp,8
		call verificar_casilla_eliminar_soldado
		add rsp,8
        cmp byte[valido],1
        je comer_soldado

    solo_mover_oficial:
        obtener_fila_columna casilla_destino,fila,columna

    	sub rsp,8
		call verificar_casilla_vacia
		add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_oficial

        ;------------
        sub rsp,8
        call activar_flag_quien_puede_comer
        add rsp,8
        mov byte[tiene_que_comer],0
        obtener_fila_columna casilla_partida,fila_partida,columna_partida
        ;------------

        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        
        
        sub rsp,8
        call verificar_movimiento_oficial_valido
        add rsp,8
        ;------------


        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

actualizar_posicion_oficial:
	;ACTUALIZO LA POSICION DEL OFICIAL ELEGIDO
        mov ax, [casilla_destino]
        ;chequear si mi casilla_partida pertenece a mi oficial1 o a mi oficial2
        sub rsp,8
        call verificar_que_oficial_soy
        add rsp,8
        cmp byte[oficial_1o2], 1
        je oficial1
        mov [posicion_oficial2], ax
        jmp bp_eliminar_oficial
        oficial1:
        mov [posicion_oficial1], ax

	bp_eliminar_oficial:
        cmp byte[oficial_por_cumplir], 0
        jne eliminar_oficial

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

        ; al comer soldado, se le pasa al esi el vector/contador del oficial que realizo el movimiento 
        inc word[r12d + 16]
eliminar_soldado:
        movzx rax, word[fila_destino]; inicializo i filas
        movzx rcx, word[columna_destino];inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia
;mover oficial
        movzx rax, word[fila_destino]   ; calculo i filas
        movzx rcx, word[columna_destino]   ;calculo j columnas

        imul rax, rax, 2
        sub ax, [fila_partida]
        mov [fila_destino], ax

        imul rcx, rcx, 2
        sub cx, [columna_partida]
        mov [columna_destino], cx

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

        dec byte[cantidad_soldados]

        mov al ,byte[fila_destino]
        add al, '0'
        mov byte[casilla_destino], al
        mov al ,byte[columna_destino]
        add al, '0'
        mov byte[casilla_destino+1], al

        jmp actualizar_posicion_oficial

    cambio_turno_oficial:
        mov byte[turno], 0
		mov byte[tiene_que_comer], 0
        jmp validar_estadisticas
;------------------------------------------------------------------SI EL OFICIAL NO CUMPLE CON SU DEBER

   eliminar_oficial:
        cmp byte[oficial_por_cumplir], 1
        je eliminar_oficial1
        cmp byte[oficial_por_cumplir], 2
        je eliminar_oficial2
        cmp byte[oficial_por_cumplir], 3
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

        mov word[posicion_oficial1],"00"
      	jmp cambio_turno_oficial
   eliminar_oficial2:
        obtener_fila_columna posicion_oficial2, fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]

        mov word[posicion_oficial2],"00"
      	jmp cambio_turno_oficial
   eliminar_oficiales:
        obtener_fila_columna casilla_destino , fila, columna

       	movzx rax, word[fila]   ; inicializo i filas
        movzx rcx, word[columna]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        dec byte[cantidad_oficiales]
        sub rsp,8
        call verificar_que_oficial_soy
        add rsp,8
        cmp byte[oficial_1o2],1
        je actualizar_oficial_1
        mov word[posicion_oficial2],"00"
      	jmp cambio_turno_oficial
        actualizar_oficial_1:
            mov word[posicion_oficial1],"00"
      	jmp cambio_turno_oficial


;------------------------------------------------------------------

    obtener_adyacencias:; [sup, sup_i, sup_d, medio_i, medio_d, inf, inf_i, inf_d]
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

    obtener_dir_celda:;cargar fila en ax y columna en bx
        movzx rax, ax ; inicializo i filas
        movzx rbx, bx  ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rbx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        ;add rax,qword[matriz]
        ret ;devuelve en rax la direccion de la celda
    ;------------------------------------------------------------------
    obtener_dir_celdas:;rsi dir del buffer, rdi dir del nuevo buffer, rcx cantidad de celdas a traducir

        traducir:
            movzx ax,[rsi]    ;fila
            movzx bx,[rsi+1]  ;columna

            sub rsp,8
            call obtener_dir_celda ;traducir celda
            add rsp,8

            mov [rdi],ax

            add rdi,2  ;dir siguiente celda nuevo buffer
            add rsi,2  ;dir siguiente celda viejo buffer
            loop traducir

        ret

    ;------------------------------------------------------------------

    verificar_movimiento_oficial_valido: ;si podia comer y no lo hizo => RIP oficial
        mov r10, 0

        sub rsp,8
        call obtener_adyacencias
        add rsp,8

        sub rsp,8
        mov rsi,buffer_casillas_ady
        mov rdi,buffer_casillas_ady_trad
        mov rcx,8
        call obtener_dir_celdas
        add rsp,8

        mov rsi,buffer_casillas_ady
        mov rdi,buffer_casillas_ady_trad
        mov rcx,8
        checkear_celda:
            mov bl,0
            movzx rax,word[rdi]
            cmp [matriz+rax],bl ;adyacencia estaria fuera de matriz
            je siguiente_loop


            mov bl,byte[vacio]
            movzx rax,word[rdi]
            cmp [matriz+rax],bl ;esta vacia
            je siguiente_loop

            mov bl,byte[num_oficial]
            movzx rax,word[rdi]
            cmp [matriz+rax],bl ;hay un oficial
            je siguiente_loop
            ;celda siguiente a adyacente ady = (part + dest)/2 => dest = (2 * ady) - part => thanks agus

            ;-------------  updated => casilla ady
            movzx ax,[rsi]
            movzx bx,[rsi+1]
            ;-------------  outdated => era para usar la casilla siguiente a la adyacente
            movzx ax,[rsi]
            imul ax,2
            sub ax,[fila_partida]

            cmp ax,0
            jl siguiente_loop
            cmp ax,6
            jg siguiente_loop

            movzx bx,[rsi+1]
            imul bx,2
            sub bx,[columna_partida]
            ;-------------

            cmp bx,0
            jl siguiente_loop
            cmp bx,6
            jg siguiente_loop



            ;traducir a desface
            sub rsp,8
            call obtener_dir_celda
            add rsp,8

            ;checkear si esta vacia => habia un movimiento mejor
            mov bl,byte[vacio]
            cmp [matriz+rax],bl
            je puede_comer ; vacia=> puede comer esto, agregar a movimientos optimos para desp comparar con input

            siguiente_loop:
            ;sigo con siguiente celda
                add rsi,2
                add rdi,2
                dec rcx
                cmp rcx,0
                je optimos_completados
                jmp checkear_celda
        ;si llega aca => ya tengo todos los optimos
        optimos_completados:
        ret

        puede_comer: ;podia "comer" un soldado y no lo hizo, agrego a "movimientos_optimos"
            mov byte[tiene_que_comer],1 ;por lo menos una que podia comer

            mov al,[rsi]
            ;------------;outdated => era para usar la casilla siguiente a la adyacente
            ;add al,al
            ;sub al,byte[fila_partida]
            ;------------

            mov ah,[rsi+1]
            ;------------;outdated => era para usar la casilla siguiente a la adyacente
            ;add ah,ah
            ;sub ah,byte[columna_partida]
            ;------------




            mov [movimientos_optimos+r10],ax
            add r10,2
            jmp siguiente_loop


    ;------------------------------------------------------------------

    verificar_movimiento_soldado_valido:
        partida_en_fuerte:
            sub rsp,8
            mov ax,word[fila_partida]
            mov word[fila],ax

            mov ax,word[columna_partida]
            mov word[columna],ax

            call verificar_celda_en_fuerte
            add rsp,8
            cmp rax,1
            je destino_en_fuerte
            jne movimiento_fuera_fuerte
        destino_en_fuerte:
            sub rsp,8
            mov ax,word[fila_destino]
            mov word[fila],ax

            mov ax,word[columna_destino]
            mov word[columna],ax
            call verificar_celda_en_fuerte
            add rsp,8
            cmp rax,1
            je movimiento_dentro_fuerte
            jne movimiento_fuera_fuerte
        movimiento_dentro_fuerte:
        ret
        movimiento_fuera_fuerte:

        cmp byte[fila_destino],6
        jg movimiento_soldado_invalido

        cmp byte[fila_destino],1
        jl movimiento_soldado_invalido

        cmp byte[columna_destino],6
        jg movimiento_soldado_invalido

        cmp byte[columna_destino],0
        jl movimiento_soldado_invalido

        cmp byte[fila_partida],4
        je validar_mov_horizontal
        jne validar_mov_vertical_diagonal

        validar_mov_horizontal:

            cmp byte[columna_partida],2
            jl lado_izq
            cmp byte[columna_partida],4
            jg lado_der
            jmp validar_mov_vertical_diagonal
            lado_izq:
                mov al,[columna_partida]
                cmp al,[columna_destino]
                jge movimiento_soldado_invalido
            mov al,[fila_partida]
            cmp al,[fila_destino]
            mov rdi, txt_movimiento_soldado_horizontal
            jne movimiento_soldado_invalido
            ret
            lado_der:
                mov al,[columna_partida]
                cmp al,[columna_destino]
                jle movimiento_soldado_invalido
            mov al,[fila_partida]
            cmp al,[fila_destino]
            mov rdi, txt_movimiento_soldado_horizontal
            jne movimiento_soldado_invalido
            ret

        validar_mov_vertical_diagonal:
            mov al,[fila_partida]
            cmp al,[fila_destino]
            mov rdi, txt_movimiento_soldado_dir_fortaleza
            jge movimiento_soldado_invalido

        ret
        movimiento_soldado_invalido:
            mov byte[valido],0 ;0 invalido, 1 valido
            sub rsp,8
            call printf
            add rsp,8

        ret
    verificar_celda_en_fuerte:      ;cargar en fila y columna. 0 no esta dentro.1 esta dentro.
        cmp byte[fila],4
        jl no_esta_en_fuerte
        cmp byte[columna],2
        jl no_esta_en_fuerte
        cmp byte[columna],5
        jg no_esta_en_fuerte
        mov rax,1
        ret
        no_esta_en_fuerte:
            mov rax,0
        ret

    activar_flag_quien_puede_comer:;1 si of_1 puede comer, 2 si of_2 puede_comer, 3 si ambos pueden comer
        mov byte[oficial_por_cumplir],0
        obtener_fila_columna posicion_oficial1,fila_partida,columna_partida
        sub rsp,8
        call verificar_movimiento_oficial_valido
        add rsp,8
        mov al,[tiene_que_comer]
        mov [tiene_que_comer_1],al
        mov byte[tiene_que_comer],0
        obtener_fila_columna posicion_oficial2,fila_partida,columna_partida
        sub rsp,8
        call verificar_movimiento_oficial_valido
        add rsp,8
        mov al,[tiene_que_comer]
        mov [tiene_que_comer_2],al

        cmp al,[tiene_que_comer_1]
        jg activar_oficial_2
        jl activar_oficial_1
        cmp byte[tiene_que_comer_1],1
        je ambos_cumplen
        mov byte[tiene_que_comer],0
        ret
        activar_oficial_1:
            mov byte[oficial_por_cumplir],1
        ret
        activar_oficial_2:
            mov byte[oficial_por_cumplir],2
        ret
        ambos_cumplen:
            mov byte[oficial_por_cumplir],3
        ret




