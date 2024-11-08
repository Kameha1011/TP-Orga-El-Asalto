#!/bin/bash

# Verificar que se haya pasado exactamente un argumento
if [ $# -eq 1 ]; then
    # Ensamblar el archivo .asm con NASM
    nasm -f elf64 -g -F dwarf -l $1.lst -o $1.o $1.asm

    # Vincular el archivo objeto con GCC y generar el ejecutable
    gcc -no-pie -o $1.out $1.o
else
    echo "Faltó ingresar el nombre del archivo.asm (sin extensión)"
fi