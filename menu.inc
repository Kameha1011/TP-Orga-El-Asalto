%include "movimientos.inc"
%include "verificaciones.inc"
global menu_principal


section .data

    txt_menu_principal db "Menú principal",10,0
    txt_menu_principal_opciones db "1-Continuar partida   |   2-Nueva partida   |   3-Salir del juego",10,0
    txt_continuar_partida db "Continuar partida",10,0
    txt_menu_nueva_partida db "Nueva partida",10,0
    txt_menu_nueva_partida_opciones db "1-Personalizar partida   |   2-Empezar partida por defecto   |   3-Volver atras   |   4-Salir del juego",10,0
    txt_menu_personalizar_partida db "Personalizar partida - Modificar",10,0
    txt_menu_personalizar_partida_opciones db "1-Simbolo de soldados   |   2-Simbolo de oficiales   |   3-Orientacion de tablero(arriba por defecto)  ",10,"4-Quien empieza(soldados por defecto)   |   5-Comenzar   |   6-Volver atras   |   7-Salir del juego",10,0
    txt_simbolo_soldados db "Ingrese el simbolo de los soldados: ",0
    txt_simbolo_oficiales db "Ingrese el simbolo de los oficiales: ",0
    txt_orientacion_tablero db "Elija la orientacion del tablero (arriba por defecto): ",0
    txt_menu_orientacion_tablero db "1-Arriba   |   2-Abajo   |   3-Izquierda   |   4-Derecha   | 5-Volver",10,0
    txt_quien_empieza db "1-Empiezan los soldados   |   2-Empiezan los oficiales   | 3-Volver",0



    txt_menu_juego db "Menú de juego",10,0

    txt_turno_soldado db "1-Elige un soldado   |   2-Salir del juego",10,0
    txt_mover_soldado db "1-Mover soldado      |   2-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas.
    txt_turno_oficial db "1-Elige un oficial   |   2-Salir del juego",10,0
    txt_mover_oficial db "1-Mover oficial      |   2-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas. (ver el caso si se saltó una captura)
    txt_salir_juego db "Saliendo ...",10,0

    txt_error_ingreso db "Error en ingreso",10,0
    txt_casilla_invalida db "Casilla inválida",10,0

    txt_casilla db "Ingrese la casilla (XX): ",0
    txt_opcion db "Ingrese la opción: ",0

    ;saltar turno (preguntar a los profes) || opcion de rendirse en caso borde 1
	formato_ingreso db "%s",0

section .bss
    opcion resb 1
	valido resb 1 ; 0 para invalido y 1 para valido
    buffer resb 1
section .text
    menu_principal:
    ;Imprimir 'Menu principal'
        mov rdi,txt_menu_principal
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_principal_opciones ;"1-Continuar partida   |   2-Nueva partida   |   3-Salir del juego"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8
        cmp byte[opcion],'1'
        je menu_continuar_partida
        cmp byte[opcion],'2'
        je menu_nueva_partida
        cmp byte[opcion],'3'
        je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp menu_principal

;---------------------------------------------------------------------
    menu_continuar_partida:
        ;Imprimir 'Continuar partida' (no implementado) -> a revisar guardado de partidas, etc
        mov rdi,txt_continuar_partida
        sub rsp,8
        call printf
        add rsp,8

        jmp menu_principal; esto solo porque no esta implementado

;----------------------------------------------------------------------
    menu_nueva_partida:
        ;Imprimir 'Nueva partida'
        mov rdi,txt_menu_nueva_partida
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_nueva_partida_opciones ;"1-Personalizar partida   |   2-Empezar partida por defecto   |   3-Volver atras   |   4-Salir del juego"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],'1'
        je menu_personalizar_partida
        cmp byte[opcion],'2'
        je empezar_juego
        cmp byte[opcion],'3'
        je menu_principal
        cmp byte[opcion],'4'
        je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp menu_nueva_partida
;----------------------------------------------------------------------
    menu_personalizar_partida:
        ;Imprimir 'Personalizar partida'
        mov rdi,txt_menu_personalizar_partida
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_personalizar_partida_opciones ;"1-Simbolo de soldados   |   2-Simbolo de oficiales   |   3-Orientacion de tablero(arriba por defecto)  ",10
                                                           ;"4-Quien empieza(soldados por defecto)   |   5-Comenzar   |   6-Volver atras   |   7-Salir del juego"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],'1'
        je personalizar_simbolo_soldados
        cmp byte[opcion],'2'
        je personalizar_simbolo_oficiales
        cmp byte[opcion],'3'
        je personalizar_orientacion_tablero
        cmp byte[opcion],'4'
        je quien_empieza
        cmp byte[opcion],'5'
        je empezar_juego
        cmp byte[opcion],'6'
        je menu_nueva_partida
        cmp byte[opcion],'7'
        je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp menu_personalizar_partida

;---------------------------------------------------------------------
    personalizar_simbolo_soldados:
        ;Imprimir 'Personalizar simbolo de soldados'
        mov rdi,txt_menu_principal    ;"Ingrese el simbolo de los soldados: "
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        mov ax, [opcion]
        mov [num_soldado], ax

        jmp menu_personalizar_partida

;----------------------------------------------------------------------
    personalizar_simbolo_oficiales:
        ;Imprimir 'Personalizar simbolo de oficiales'
        mov rdi, txt_simbolo_oficiales; "Ingrese el simbolo de los oficiales: "
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        mov ax, [opcion]
        mov [num_oficial], ax

        jmp menu_personalizar_partida

;----------------------------------------------------------------------
    personalizar_orientacion_tablero:
        ;Imprimir 'Personalizar orientacion de tablero'
        mov rdi, txt_orientacion_tablero ;"Elija la orientacion del tablero (arriba por defecto): "
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_orientacion_tablero ;"1-Arriba   |   2-Abajo   |   3-Izquierda   |   4-Derecha   | 5-Volver"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],'1'
        ;je orientacion_arriba
        cmp byte[opcion],'2'
        ;je orientacion_abajo
        cmp byte[opcion],'3'
        ;je orientacion_izquierda
        cmp byte[opcion],'4'
        ;je orientacion_derecha
        cmp byte[opcion],'5'
        je menu_personalizar_partida

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp personalizar_orientacion_tablero
;----------------------------------------------------------------------
    quien_empieza:
        ;Imprimir 'Quien empieza'
        mov rdi, txt_quien_empieza ;"1-Empiezan los soldados   |   2-Empiezan los oficiales   | 3-Volver"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],'1'
        je empiezan_soldados
        cmp byte[opcion],'2'
        je empiezan_oficiales
        cmp byte[opcion],'3'
        je menu_personalizar_partida

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp quien_empieza

    empiezan_soldados:
        mov byte[turno],0
        jmp menu_personalizar_partida
    empiezan_oficiales:
        mov byte[turno],1
        jmp menu_personalizar_partida

;----------------------------------------------------------------------
    empezar_juego:
        ;Imprimir 'Menu de juego'
        mov rdi,txt_menu_juego
        sub rsp,8
        call printf
        add rsp,8

        ;Imprimir 'Elegir soldado' o 'Elegir oficial' depende del turno
        cmp byte[turno],0
        je txt_elegir_soldado
        cmp byte[turno],1
        je txt_elegir_oficial

;----------------------------------------------------------------
    txt_elegir_soldado: ;"1-Elige un soldado  |   2-Salir del juego"
        mov rdi, txt_turno_soldado
        sub rsp,8
        call printf
        add rsp,8

    	sub rsp,8
        call leer_opcion
		add rsp,8

        cmp byte[opcion],'1'
       	je elegir_soldado
       	cmp byte[opcion],'2'
       	je salir_juego

    	sub rsp,8
        call error_ingreso
		add rsp,8
        jmp txt_elegir_soldado
;----------------------------------------------------------------
    elegir_soldado:
        sub rsp,8
    	call leer_casilla
    	add rsp,8
		sub rsp,8
		call verificar_casilla_soldado
		add rsp,8

		cmp byte[valido],0
		je casilla_invalida_soldado

		;si es valido
    	sub rsp,8
    	call obtener_adyacencias
        add rsp,8
    	mov ax, [fila]
        mov [fila_partida],ax
        mov ax, [columna]
        mov [columna_partida],ax

	pedir_casilla_destino_soldado:
        sub rsp,8
    	call mover_soldado
    	add rsp,8

    	cmp byte[opcion],"2"
    	je salida

        mov rdi, casilla_destino
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8
        sub rsp,8
        call verificar_casilla_destino_adyacente
        add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_soldado
       	sub rsp,8
    	call verificar_casilla_vacia
    	add rsp,8

      	cmp byte[valido],1
		je movimiento_soldado
    	cmp byte[valido],0
    	je pedir_casilla_destino_soldado



;-----------------------------------------------------------------


	mover_soldado: ;"1-Mover soldado      |   2-Salir del juego"
        ;Imprimir 'Mover soldado'
        mov rdi, txt_mover_soldado
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
		add rsp,8

        cmp byte[opcion],"1"
       	je leer_casilla
        cmp byte[opcion],"2"
       	je salir_juego

		sub rsp,8
		call error_ingreso
		add rsp,8
		jmp txt_elegir_soldado
;----------------------------------------------------------------

    txt_elegir_oficial: ;"1-Elige un oficial  |   2-Salir del juego"
        mov rdi, txt_turno_oficial
        sub rsp,8
        call printf
        add rsp,8

    	sub rsp,8
        call leer_opcion
		add rsp,8

        cmp byte[opcion],"1"
       	je elegir_oficial
       	cmp byte[opcion],"2"
       	je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp txt_elegir_oficial

;----------------------------------------------------------------

    elegir_oficial:
        sub rsp,8
    	call leer_casilla
    	add rsp,8
    	sub rsp,8
    	call verificar_casilla_oficial
        add rsp,8

    	cmp byte[valido],0
    	je casilla_invalida_oficial
        ;si es valido
    	mov ax, [fila]
        mov [fila_partida],ax
        mov ax, [columna]
        mov [columna_partida],ax
	pedir_casilla_destino_oficial:
		sub rsp,8
		call mover_oficial
		add rsp,8

		cmp byte[opcion],"2"
    	je salida

        mov rdi, casilla_destino
        sub rsp, 8
        call obtener_fila_columna
        add rsp, 8
        sub rsp,8
        call verificar_casilla_destino_adyacente
        add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_oficial
        
		sub rsp,8
		call verificar_casilla_vacia
		add rsp,8

		cmp byte[valido],1
		je movimiento_oficial
		cmp byte[valido],0
		je pedir_casilla_destino_oficial


;----------------------------------------------------------------

    mover_oficial: ;"1-Mover oficial      |   2-Salir del juego"
        ;Imprimir 'Mover oficial'
        mov rdi, txt_mover_oficial
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],"1"
       	je leer_casilla
        cmp byte[opcion],"2"
       	je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
		jmp txt_elegir_oficial
;----------------------------------------------------------------
 	leer_opcion:
    ;Imprimir 'Ingrese la opcion: '
    	mov rdi, txt_opcion
       	sub rsp,8
    	call printf
        add rsp,8
    ;Leer opcion
    	mov rdi, formato_ingreso
    	lea rsi, [buffer]
        sub rsp,8
        call scanf
        add rsp,8
        mov bh, [buffer]
        mov [opcion], bh

        cmp rax, 1
        jl error_scan
    	ret
    error_scan:
        mov byte[opcion],"3"
        ret
;----------------------------------------------------------------
    leer_casilla:
    ;Imprimir 'Ingrese la casilla: '
    	mov rdi, txt_casilla
       	sub rsp,8
    	call printf
        add rsp,8
    ;Leer casilla
        mov rdi, formato_ingreso
    	lea rsi, [buffer]
        sub rsp,8
        call scanf
        add rsp,8

        mov bx, [buffer]
        mov [casilla], bx
        mov [casilla_destino], bx

        cmp rax, 1
        jl error_scan_casilla
    	ret
    error_scan_casilla:
        mov word[casilla],"11"
        ret

;----------------------------------------------------------------
    salir_juego:
        ;Imprimir 'Salir del juego'
        mov rdi, txt_salir_juego
        sub rsp,8
        call printf
        add rsp,8
        mov rax, [sys_call]
        ret
;----------------------------------------------------------------

    error_ingreso:
        ;Imprimir 'Error en ingreso'
        mov rdi, txt_error_ingreso
        sub rsp,8
        call printf
        add rsp,8
		ret
;----------------------------------------------------------------

	casilla_invalida_oficial:
		sub rsp,8
		call casilla_invalida
		add rsp,8
		jmp txt_elegir_oficial

;----------------------------------------------------------------
    casilla_invalida_soldado:
		sub rsp,8
		call casilla_invalida
		add rsp,8
		jmp txt_elegir_soldado

;----------------------------------------------------------------
    casilla_invalida:
        ;Imprimir 'Casilla invalida'
        mov rdi, txt_casilla_invalida
        sub rsp,8
        call printf
        add rsp,8
        ret
;----------------------------------------------------------------
    ; Subroutine to clear the input buffer
    clear_input_buffer:
        mov rax, 0              ; Syscall for read
        mov rdi, 0              ; File descriptor for standard input (stdin)
        lea rsi, [buffer]       ; Buffer to store discarded input
        mov rdx, 1              ; Read one byte at a time
    clear_loop:
        syscall                 ; Read one byte
        cmp byte [buffer], 10   ; Check if it's a newline character (ASCII 10)
        jne clear_loop          ; Repeat until newline is found
        ret                     ; Return to caller
;----------------------------------------------------------------
	salida:
        ret
;----------------------------------------------------------------

