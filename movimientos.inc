global movimientos

section .data

section .bss

    fila_partida resw 1
    columna_partida resw 1
    fila_destino resw 1
    columna_destino resw 1
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

        movzx rax, word[fila_destino]   ; inicializo i filas
        movzx rcx, word[columna_destino]   ;inicializo j columnas

        imul rax,7      ;i filas * cant columnas
        add  rax,rcx     ;(i filas * cant columnas) + j columnas
        imul rax,1       ;((i filas * cant columnas) + j columnas) * long elemento

        mov bl, [num_oficial]
        mov byte[matriz + rax], bl ;marco la casilla de destino con el oficial

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

            ;movzx rax, word[rsi]

            movzx ax,[rsi]
            imul ax,2
            sub ax,[fila_partida]

            movzx bx,[rsi+1]
            imul bx,2
            sub bx,[columna_partida]
          
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
        ;si llega aca => movimiento valido
        ret

        puede_comer: ;podia "comer" un soldado y no lo hizo, agrego a "movimientos_optimos"
    
            mov al,[rsi]
            add al,al
            sub al,byte[fila_partida]

            mov ah,[rsi+1]
            add ah,ah
            sub ah,byte[columna_partida]

            
            mov [movimientos_optimos+r10],ax
            add r10,2
            jmp siguiente_loop
        

    ;------------------------------------------------------------------



