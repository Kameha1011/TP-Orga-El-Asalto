global abrir_archivo_lectura_escritura

extern  puts
extern  fopen
extern  fwrite
extern  fread
extern  fclose

section .data
    nombre_archivo db "juego.dat", 0           ; Nombre del archivo
    modo_escritura db "wb", 0                   ; Modo escritura binaria
    modo_lectura db "rb", 0                     ; Modo lectura binaria
    msg_error_abrir db "No hay una partida guardada", 0
section .bss
    buffer_archivo resq 1                   ; Descriptor de archivo (8 bytes)
section .text
abrir_archivo:
    mov rdi, nombre_archivo                   ; Nombre del archivo
    sub rsp,8
    call fopen                          ; Abrir el archivo
    add rsp,8
    cmp	rax,0					;Error en apertura?
	jg	end_abrir_archivo
    mov rdi, msg_error_abrir                 ; Mensaje de error
    sub rsp,8
    call puts
    add rsp,8
    mov rcx,0 ;para saber si hubo error
    ret
end_abrir_archivo:
    mov [buffer_archivo], rax               ; Guardar el handle del archivo
    ret

guardar_partida:
    mov rsi, modo_escritura                 ; Modo escritura
    sub rsp,8
    call abrir_archivo
    add rsp,8
    cmp rcx,0
    je end_guardar
    mov rdi, struct_partida             ; Dirección de la estructura
    mov rsi, 78                         ; Tamaño total de la estructura (calcular manualmente)
    mov rdx, 1                          ; Cantidad de bloques a escribir
    mov rcx, [buffer_archivo]           ; Handle del archivo
    sub rsp,8
    call fwrite                         ; Escribir al archivo
    add rsp,8
    sub rsp,8
    call cerrar_archivo
    add rsp,8
    end_guardar:
    ret

cargar_partida:
    mov rsi, modo_lectura                ; Modo escritura
    sub rsp,8
    call abrir_archivo
    add rsp,8
    cmp rcx,0
    je end_cargar
    mov rdi, struct_partida             ; Dirección donde se copiará la estructura
    mov rsi, 78                         ; Tamaño total de la estructura
    mov rdx, 1                          ; Cantidad de bloques a leer
    mov rcx, [buffer_archivo]           ; Handle del archivo
    sub rsp,8
    call fread                          ; Leer desde el archivo
    add rsp,8
    sub rsp,8
    call cerrar_archivo
    add rsp,8
    end_cargar:
    ret

cerrar_archivo:
    mov rdi, [buffer_archivo]               ; Handle del archivo
    sub rsp,8
    call fclose                         ; Cerrar el archivo
    add rsp,8
    ret