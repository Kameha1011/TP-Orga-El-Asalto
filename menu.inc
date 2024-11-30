%include "movimientos.inc"
%include "verificaciones.inc"
%include "guardado_partida.inc"
global menu_principal

extern system
section .data
    cmd_clear db "clear",0

    txt_menu_principal db "Menú principal",10,0
    txt_menu_principal_opciones db "1-Continuar partida   |   2-Nueva partida   |   3-Salir del juego",10,0
    txt_continuar_partida db "Continuar partida",10,0
    txt_menu_nueva_partida db "Nueva partida",10,0
    txt_menu_nueva_partida_opciones db "1-Personalizar partida   |   2-Empezar partida por defecto   |   3-Volver atras   |   4-Salir del juego",10,0
    txt_menu_personalizar_partida db "Personalizar partida - Modificar",10,0
    txt_menu_personalizar_partida_opciones db "1-Simbolo de soldados   |   2-Simbolo de oficiales   |   3-Orientacion de tablero(arriba por defecto)  ",10,"4-Quien empieza(soldados por defecto)   |   5-Comenzar   |   6-Volver atras   |   7-Salir del juego",10,0
    txt_simbolo_soldados db "Ingrese el simbolo de los soldados: ",10,0
    txt_simbolo_oficiales db "Ingrese el simbolo de los oficiales: ",10,0
    txt_orientacion_tablero db "Elija la orientacion del tablero (arriba por defecto): ",10,0
    txt_menu_orientacion_tablero db "1-Arriba   |   2-Abajo   |   3-Izquierda   |   4-Derecha   | 5-Volver",10,0
    txt_quien_empieza db "1-Empiezan los soldados   |   2-Empiezan los oficiales   | 3-Volver",10,0



    txt_menu_juego db "Menú de juego",10,0

    txt_turno_soldado db "1-Elige un soldado   |   2-Salir del juego   |   3-Guardar y Salir",10,0
    txt_mover_soldado db "1-Mover soldado      |   2-Volver atras   |   3-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas.
    txt_turno_oficial db "1-Elige un oficial   |   2-Salir del juego   |   3-Guardar y Salir",10,0
    txt_mover_oficial db "1-Mover oficial      |   2-Volver atras   |   3-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas. (ver el caso si se saltó una captura)
    txt_salir_juego db "Saliendo ...",10,0

    txt_error_ingreso db "Error en ingreso",10,0
    txt_casilla_invalida db "Casilla inválida",10,0

    txt_casilla db "Ingrese la casilla (XX) - Solo toma los 2 primeros digitos : ",0
    txt_opcion db "Ingrese la opción - Solo toma el primer digito: ",0

	formato_ingreso db "%s",0

section .bss
    opcion resb 1
	valido resb 1 ; 0 para invalido y 1 para valido
    buffer resb 1
section .text
    menu_principal:
    	sub rsp, 8
    	call limpiar_pantalla
        add rsp, 8
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
        je continuar_partida
        cmp byte[opcion],'2'
        je menu_nueva_partida
        cmp byte[opcion],'3'
        je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp menu_principal

;----------------------------------------------------------------------
    menu_nueva_partida:
    	sub rsp, 8
    	call limpiar_pantalla
        add rsp, 8

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
        je partida_por_defecto
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
        sub rsp, 8
    	call limpiar_pantalla
        add rsp, 8

        ;Imprimir 'Personalizar partida'
        mov rdi,txt_menu_personalizar_partida
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_personalizar_partida_opciones ;"1-Simbolo de soldados   |   2-Simbolo de oficiales   |   3-Orientacion de tablero(arriba por defecto)  "
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
        je juego
        cmp byte[opcion],'6'
        je menu_nueva_partida
        cmp byte[opcion],'7'
        je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp menu_personalizar_partida

;---------------------------------------------------------------------
    partida_por_defecto:
    	mov byte[simbolo_soldado],'X'
    	mov byte[simbolo_oficial],'O'
       	mov byte[orientacion_tablero],1
    	mov byte[turno],1

    	jmp juego

;---------------------------------------------------------------------
    personalizar_simbolo_soldados:
        ;Imprimir 'Personalizar simbolo de soldados'
        mov rdi,txt_menu_principal    ;"Ingrese el simbolo de los soldados: "
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_casilla
        add rsp,8

        mov ax, [casilla]
        mov [simbolo_soldado], ax

        jmp menu_personalizar_partida

;----------------------------------------------------------------------
    personalizar_simbolo_oficiales:
        ;Imprimir 'Personalizar simbolo de oficiales'
        mov rdi, txt_simbolo_oficiales; "Ingrese el simbolo de los oficiales: "
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_casilla
        add rsp,8

        mov ax, [casilla]
        mov [simbolo_oficial], ax

        jmp menu_personalizar_partida

;----------------------------------------------------------------------
    personalizar_orientacion_tablero:
    	sub rsp, 8
    	call limpiar_pantalla
        add rsp, 8

        ;Imprimir 'Personalizar orientacion de tablero'
        mov rdi, txt_orientacion_tablero ;"Elija la orientacion del tablero (arriba por defecto): "
        sub rsp,8
        call printf
        add rsp,8

        mov rdi, txt_menu_orientacion_tablero ;"1-Abajo(por defecto)   |   2-Arriba   |   3-Izquierda   |   4-Derecha   | 5-Volver"
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],'1'
        je orientacion_abajo
        cmp byte[opcion],'2'
        je orientacion_arriba
        cmp byte[opcion],'3'
        je orientacion_izquierda
        cmp byte[opcion],'4'
        je orientacion_derecha
        cmp byte[opcion],'5'
        je menu_personalizar_partida

        sub rsp,8
        call error_ingreso
		add rsp,8
        jmp personalizar_orientacion_tablero

      orientacion_arriba:
        mov byte[orientacion_tablero],1
        jmp menu_personalizar_partida
   	  orientacion_abajo:
        mov byte[orientacion_tablero],2
        jmp menu_personalizar_partida
      orientacion_izquierda:
        mov byte[orientacion_tablero],3
        jmp menu_personalizar_partida
      orientacion_derecha:
    	mov byte[orientacion_tablero],4
        jmp menu_personalizar_partida
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
    juego:

        sub rsp, 8
    	call limpiar_pantalla
        add rsp, 8
    	sub rsp, 8
		call elegir_orientacion
	    add rsp,8

    	sub rsp, 8
        call empezar_juego
        add rsp, 8

    	cmp rax,[sys_call]
        je fin

        sub rsp, 8 
        call verificar_fuerte
        add rsp, 8


        cmp byte[soldados_en_fuerte],9
        je fin_fuerte
        cmp byte[cantidad_soldados],8
        je fin_soldado
        cmp byte[cantidad_oficiales],0
        je fin_oficial
        jmp juego

        ret
    fin:
        mov rax,[sys_call]
        mov rdi,[exit_code]
        syscall
    fin_oficial:
        sub rsp, 8
        call elegir_orientacion
        add rsp,8
        mov rdi, oficiales_insuficientes
        sub rsp, 8
        call printf
        add rsp, 8

        mov rax,[sys_call]
        mov rdi,[exit_code]
        syscall
    fin_soldado:
        sub rsp, 8
        call elegir_orientacion
        add rsp,8
        mov rdi, soldados_insuficientes
        sub rsp, 8
        call printf
        add rsp, 8

        mov rax,[sys_call]
        mov rdi,[exit_code]
        syscall
    fin_fuerte:
        sub rsp, 8
        call elegir_orientacion
        add rsp,8
        mov rdi, fuerte_tomado
        sub rsp, 8
        call printf
    	add rsp, 8

    	mov rax,[sys_call]
    	mov rdi,[exit_code]
    	syscall

;----------------------------------------------------------------------
    empezar_juego:
        ;Imprimir 'Menu de juego'
        mov rdi,txt_menu_juego
        sub rsp,8
        call printf
        add rsp,8
    bifurcar_a_turno:
        ;Imprimir 'Elegir soldado' o 'Elegir oficial' depende del turno
        cmp byte[turno],0
        je txt_elegir_soldado
        cmp byte[turno],1
        je txt_elegir_oficial

;----------------------------------------------------------------
    txt_elegir_soldado: ;"1-Elige un soldado  |   2-Salir del juego   |  3-Guardar y Salir" 
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
        cmp byte[opcion],"3"
        je guardar_y_salir

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

    	cmp byte[opcion],"3"
    	je salida

       	obtener_fila_columna casilla_destino, fila_destino, columna_destino
        sub rsp,8
        call verificar_casilla_destino_adyacente
        add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_soldado
    	obtener_fila_columna casilla_destino, fila, columna
		sub rsp,8
    	call verificar_casilla_vacia
        add rsp,8
		cmp byte[valido],0

    	je pedir_casilla_destino_soldado
      	cmp byte[valido],1
		je movimiento_soldado
    	cmp byte[valido],0
    	je pedir_casilla_destino_soldado



;-----------------------------------------------------------------


	mover_soldado: ;"1-Mover soldado      |   2-Volver atras   |   3-Salir del juego"
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
       	je txt_elegir_soldado
        cmp byte[opcion],"3"
       	je salir_juego

		sub rsp,8
		call error_ingreso
		add rsp,8
		jmp txt_elegir_soldado
;----------------------------------------------------------------

    txt_elegir_oficial: ;"1-Elige un oficial  |   2-Salir del juego   |  3-Guardar y Salir"
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
        cmp byte[opcion],"3"
        je guardar_y_salir

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
    	mov ax, [casilla]
        mov [casilla_partida],ax
    	mov ax, [fila]
        mov [fila_partida],ax
        mov ax, [columna]
        mov [columna_partida],ax

	pedir_casilla_destino_oficial:
		sub rsp,8
		call mover_oficial
		add rsp,8

		cmp byte[opcion],"3"
    	je salida

		obtener_fila_columna casilla_destino, fila_destino, columna_destino
        sub rsp,8
        call verificar_casilla_destino_adyacente
        add rsp,8
    	cmp byte[valido],0
    	je pedir_casilla_destino_oficial

        ; evaluo cual es el oficial elegido
        mov ax, [casilla_partida]
        cmp [posicion_oficial1], ax
        je eleccion_oficial1

        ; guardo la posicion del oficial elegido
        eleccion_oficial2: 
        mov esi, [stats_oficial_2]
        mov byte[oficial_elegido], 2

        jmp comprobar_validez_movimiento

        eleccion_oficial1: 
        mov esi, [stats_oficial_1]
        mov byte[oficial_elegido] ,1

    comprobar_validez_movimiento:
		cmp byte[valido],1
		je movimiento_oficial
		cmp byte[valido],0
		je pedir_casilla_destino_oficial

        ;despues de ver la validez del movimiento valido las estadisticas
        sub rsp, 8
        call validar_estadisticas
        add rsp, 8
;----------------------------------------------------------------

    mover_oficial: ;"1-Mover oficial      |   2-Volver atras   |   3-Salir del juego"
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
       	je txt_elegir_oficial
        cmp byte[opcion],"3"
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
        mov byte[opcion], bh
		mov byte[buffer],'' ;limpio buffer
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
        mov byte[buffer],'' ;limpio buffer

        cmp rax, 1
        jl error_scan_casilla
    	ret
    error_scan_casilla:
        mov word[casilla],"11"
        ret
;----------------------------------------------------------------
    ;Seccion estadisticas

    validar_estadisticas:
        ; en esi llega el vector del soldado en particular a incrementar
        mov al, [fila] ; guardo la fila inicial 
        mov ah, [columna] ; guardo la columna inicial 

        ; obtengo la fila y columna de la posicion de destino
        obtener_fila_columna casilla_destino, fila, columna

        ; CASOS POSIBLES: 
        ; fila igual y columna mayor -> se mueve hacia derecha
        ; fila igual y columna menor -> se mueve hacia izquierda
        
        ; fila mayor y columna igual -> se mueve hacia arriba
        ; fila mayor y columna mayor -> se mueve a superior derecha
        ; fila mayor y columna menor -> se mueve a superior izquierda
        
        ; fila menor y columna igual -> se mueve hacia abajo
        ; fila menor y columna mayor -> se mueve a inferior derecha
        ; fila menor y columna menor -> se mueve a inferior izquierda
        

 ; Comparar filas y columnas para determinar dirección
        cmp byte[fila_destino], al
        je fila_destino_igual
        cmp byte[fila_destino], al
        jg fila_destino_mayor

    fila_destino_menor:
        cmp byte[columna_destino], ah
        je incrementar_abajo
        cmp byte[columna_destino], ah
        jg incrementar_inferior_derecha
        jl incrementar_inferior_izquierda

    fila_destino_igual:
        cmp byte[columna_destino], ah
        jg incrementar_derecha
        jl incrementar_izquierda

    fila_destino_mayor:
        cmp byte[columna_destino], ah
        je incrementar_arriba
        cmp byte[columna_destino], ah
        jg incrementar_superior_derecha
        jl incrementar_superior_izquierda

    incrementar_izquierda:
        inc byte[esi + 0]  ; Izquierda
        ret
    incrementar_derecha:
        inc byte[esi + 1]  ; Derecha
        ret
    incrementar_abajo:
        inc byte[esi + 2]  ; Abajo
        ret
    incrementar_arriba:
        inc byte[esi + 3]  ; Arriba
        ret
    incrementar_superior_derecha:
        inc byte[esi + 4] ; Superior derecha
        ret
    incrementar_superior_izquierda:
        inc byte[esi + 5] ; Superior izquierda
        ret
    incrementar_inferior_derecha:
        inc byte[esi + 6]  ; Inferior derecha
        ret
    incrementar_inferior_izquierda:
        inc byte[esi + 7] ; Inferior izquierda
        ret
;----------------------------------------------------------------
    salir_juego:
        ; Imprimir estadisticas antes de salir del juego
        sub rsp, 8
        call imprimir_estadisticas_oficiales
        add rsp, 8

        ;Imprimir 'Salir del juego'
        mov rdi, txt_salir_juego
        sub rsp,8
        call printf
        add rsp,8
        mov rax, [sys_call]
        ret
;----------------------------------------------------------------
    continuar_partida:
        sub rsp,8
        call cargar_partida
        add rsp,8
        jmp juego
;----------------------------------------------------------------
    guardar_y_salir:
        sub rsp,8
        call guardar_partida
        add rsp,8
        sub rsp,8
        call salir_juego
        add rsp,8
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
    ;imprimimos las estadisticas de los oficiales
    imprimir_estadisticas_oficiales:
        ; Imprimir estadísticas oficial 1
        mov rdi, msg_estadisticas_oficial1
        sub rsp, 8
        call printf
        add rsp, 8

        mov rsi, [stats_oficial_1]
        mov rdi, txt_movimientos_direcciones
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_movimientos_diagonales
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_movimientos_diagonales_2
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_capturas
        sub rsp, 8
        call printf
        add rsp, 8

        ; Imprimir estadísticas oficial 2
        mov rdi, msg_estadisticas_oficial2
        sub rsp, 8
        call printf
        add rsp, 8

        mov rsi, [stats_oficial_2]
        mov rdi, txt_movimientos_direcciones
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_movimientos_diagonales
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_movimientos_diagonales_2
        sub rsp, 8
        call printf
        add rsp, 8

        mov rdi, txt_capturas
        sub rsp, 8
        call printf
        add rsp, 8
        ret

