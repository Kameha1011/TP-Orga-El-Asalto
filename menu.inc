global menu

extern printf
extern scanf

section .data
    txt_menu_juego db "Menu de juego",10,0

    txt_turno_soldado db "1-Elige un soldado  |   2-Salir del juego",10,0
    txt_mover_soldado db "1-Mover soldado (recibe casilla)     |   2-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas.
    txt_turno_oficial db "1-Elige un oficial   |   2-Salir del juego",10,0
    txt_mover_oficial db "1-Mover oficial (recibe casilla)     |   2-Salir del juego",10,0 ;→calcular casillas disponibles y mostrarlas. (ver el caso si se saltó una captura)
    txt_salir_juego db "Saliendo ...",10,0

    txt_error_ingreso db "Error en ingreso",10,0

    txt_casilla db "Ingrese la casilla (XX): ",0
    txt_opcion db "Ingrese la opcion: ",0

    ;saltar turno (preguntar a los profes) || opcion de rendirse en caso borde 1
	formato_num db "%d",0

section .bss
    opcion resb 1

section .text
    menu:
        ;menu principal
        ret

;----------------------------------------------------------------------
    menu_juego:
        ;Imprimir 'Menu de juego'
        mov rdi,txt_menu_juego
        sub rsp,8
        call printf
        add rsp,8

        ;Imprimir 'Elegir soldado' o 'Elegir oficial'
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

        cmp byte[opcion],1
       	je elegir_soldado
       	cmp byte[opcion],2
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
		mov ax, [casilla]
		mov [posicion_soldado], ax
    	jmp mover_soldado
;-----------------------------------------------------------------


	mover_soldado: ;"1-Mover soldado (recibe casilla)     |   2-Salir del juego"
        ;Imprimir 'Mover soldado'
        mov rdi, txt_mover_soldado
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
		add rsp,8

        cmp byte[opcion],1
       	je leer_casilla
        cmp byte[opcion],2
       	je salir_juego

        ret
;----------------------------------------------------------------

    txt_elegir_oficial: ;"1-Elige un oficial  |   2-Salir del juego"
        mov rdi, txt_turno_oficial
        sub rsp,8
        call printf
        add rsp,8

    	sub rsp,8
        call leer_opcion
		add rsp,8

        cmp byte[opcion],1
       	je elegir_oficial
       	cmp byte[opcion],2
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
        mov ax, [casilla]
        mov [posicion_oficial],ax
    	jmp mover_oficial

;----------------------------------------------------------------

    mover_oficial: ;"1-Mover oficial (recibe casilla)     |   2-Salir del juego"
        ;Imprimir 'Mover oficial'
        mov rdi, txt_mover_oficial
        sub rsp,8
        call printf
        add rsp,8

        sub rsp,8
        call leer_opcion
        add rsp,8

        cmp byte[opcion],1
       	je leer_casilla
        cmp byte[opcion],2
       	je salir_juego

        sub rsp,8
        call error_ingreso
		add rsp,8
		jmp mover_oficial

;----------------------------------------------------------------

 	leer_opcion:
    ;Imprimir 'Ingrese la opcion: '
    	mov rdi, txt_opcion
       	sub rsp,8
    	call printf
        add rsp,8
    ;Leer opcion
    	mov rdi, formato_num
    	lea rsi, [opcion]
        sub rsp,8
        call scanf
        add rsp,8
    	ret
;----------------------------------------------------------------
    leer_casilla:
    ;Imprimir 'Ingrese la casilla: '
    	mov rdi, txt_casilla
       	sub rsp,8
    	call printf
        add rsp,8
    ;Leer casilla
        mov rdi, formato_num
        lea rsi, [casilla]
        sub rsp,8
        call scanf
        add rsp,8
        ret

;----------------------------------------------------------------
    salir_juego:
        ;Imprimir 'Salir del juego'
        mov rdi, txt_salir_juego
        sub rsp,8
        call printf
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
