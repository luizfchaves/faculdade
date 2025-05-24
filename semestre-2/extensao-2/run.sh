#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 nome-do-arquivo-sem-extensao"
    exit 1
fi

if [ ! -f "$1.c" ]; then
    echo "Error: arquivo '$1.c' não encontrado."
    exit 1
fi

gcc -o "$1" "$1.c"
if [ $? -eq 0 ]; then
    ./"$1"
else
    echo "Error: falha na compilação do arquivo '$1.c'."
fi