#!/bin/bash



gcc -o "projeto-algoritmos" "projeto-algoritmos.c"
if [ $? -eq 0 ]; then
    ./projeto-algoritmos
else
    echo "Error: falha na compilação do arquivo 'projeto-algoritmos.c'."
fi

exit 0