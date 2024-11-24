global movimientos

section .data
    tiene_que_comer db 0
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


    buffer_casillas_ady_trad resw 8
    movimientos_optimos resw 8
    optimo resw 1
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
        sub rsp, 8
        call calcular_distancia_entre_casillas
        add rsp, 8
        cmp rax, 0  ;rax = resultado de calcular la distancia
        je solo_mover_oficial
        cmp rax, 1
        jne error_casilla
        cmp byte[tiene_que_comer], 1
        je comer_soldado
        jmp error_casilla

    solo_mover_oficial:
        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia
        
        mov byte[tiene_que_comer],0
        sub rsp,8
        call verificar_movimiento_oficial_valido
        add rsp,8

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
        sub rsp, 8
        call verificar_casilla_eliminar_soldado
        add rsp, 8
        cmp byte[valido], 0
        je error_casilla

        movzx rax, word[fila_partida]   ; inicializo i filas
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del oficial como vacia

        mov rdi, casilla_destino
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8

        mov dx, [fila]
        mov [fila_destino], dx
        mov dx, [columna]
        mov [columna_destino], dx

eliminar_soldado:
        movzx rax, word[fila_destino]
        add rax, [fila_partida]
        xor rdx,rdx  ;limpio rdx para evitar contenido basura
        mov rbx, 2                ; Divisor
        div rbx
        mov [fila_partida], ax
        movzx rax, word[fila_partida]   ; inicializo i filas

        mov ax, word[columna_destino]
        add [columna_partida], ax
        mov ax, word[columna_partida]
        mov bx, 2
        div bx
        mov [columna_partida], ax
        movzx rcx, word[columna_partida]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov byte[matriz + rax], 1 ;marco la casilla del soldado como vacia


        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

        dec byte[cantidad_soldados]
        mov byte[comio?], 1
        ;cambio de turno
        mov byte[turno], 0
        ret
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
    calcular_distancia_entre_casillas:
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



            movzx ax,[rsi]
            ;imul ax,2          outdated => era para usar la casilla siguiente a la adyacente 
            ;sub ax,[fila_partida]

            movzx bx,[rsi+1]
            ;imul bx,2
            ;sub bx,[columna_partida]
          
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
                loop checkear_celda
        ;si llega aca => ya tengo todos los optimos
        mov rcx,8
        mov rdi,movimientos_optimos
        loop_activar_flag_comio:
            coincide_fila:
                mov al,[rdi]
                cmp byte[fila_destino],al
                jne siguiente_loop_comio_oficial
            
            coincide_col:
                mov ah,[rdi+1]
                cmp byte[columna_destino],ah
                je activar_flag_comio
            siguiente_loop_comio_oficial:
                add rdi,2
                loop loop_activar_flag_comio


        ret

        activar_flag_comio: ;podia comer y comio
            mov byte[tiene_que_comer],0

        puede_comer: ;podia "comer" un soldado y no lo hizo, agrego a "movimientos_optimos"
            mov byte[tiene_que_comer],1 ;por lo menos una que podia comer
            
            mov al,[rsi]
            ;add al,al          outdated => era para usar la casilla siguiente a la adyacente 
            ;sub al,byte[fila_partida]

            mov ah,[rsi+1]
            ;add ah,ah
            ;sub ah,byte[columna_partida]
            
            

            
            mov [movimientos_optimos+r10],ax
            add r10,2
            jmp siguiente_loop
        

    ;------------------------------------------------------------------



