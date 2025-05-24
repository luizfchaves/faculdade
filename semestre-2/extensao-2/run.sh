#!/bin/bash



gcc -o "projeto-extensao" "projeto-extensao.c"
if [ $? -eq 0 ]; then
    ./projeto-extensao
else
    echo "Error: falha na compilação do arquivo 'projeto-extensao.c'."
fi

exit 0