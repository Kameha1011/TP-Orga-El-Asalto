global movimientos

section .data

section .bss

    fila_partida resw 1
    columna_partida resw 1
    fila_destino resw 1
    columna_destino resw 1
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