#!/bin/bash

#primeiro validar se o user está a utilizar o sudo , caso não esteja ,retorna imediatamente.

if [ $UID != 0 ] ; then
	echo "must be called as superuser"
	exit 1
fi


# próximo passo é validar os argumentos recebidos - arguments: { private | prod | dev } e definir por omissão o private

ARG="${1:-private}" 

if [[ "$ARG" != "private" && "$ARG" != "prod" && "$ARG" != "dev" ]]; then
    echo "Invalid argument: $ARG"
    exit 1
fi



# agora vamos fazer a validação de cada um dos serviços retornando caso algum NÂO esteja ativo
# para isto vamos utilizar a instução is-active que ao contrário do status, retorna apenas o estado dessa unit e não um ficheiro de texto completo




WEB_APP_SERVICE="tvswebapp.service"
OLLAMA_SERVICE="ollama.service"


if ! systemctl is-active "$WEB_APP_SERVICE"; then
     echo "$WEB_APP_SERVICE is not active"
     exit 1

else
    echo "$WEB_APP_SERVICE is active "
fi 


if ! systemctl is-active "$OLLAMA_SERVICE"; then
     echo "$OLLAMA_SERVICE is not active"
     exit 1

else
    echo "$OLLAMA_SERVICE is active "
fi 



# para cada caso devemos realizar o seguinte:
# No private , primeiro temos de validar se o tvschat existe em /etc/nginx/sites-enabled/ , caso exista temos de remover o ficheiro e dar reload, caso contrário nada tem de ser feito porque isso é a opção default. 
case "$ARG" in 
        private)
        echo "private"
        rm -f /etc/nginx/sites-enabled/tvschat-dev  # a flag -f serve apenas para , caso não encontre (que é uma condição possivel) não lança exceção , caso encontre remove simplesmente
        systemctl reload nginx
        ;;

        prod)
        echo "prod"
         cp cw3/nginx/tvschat-prod /etc/nginx/sites-available/tvschat-dev    
        ln -sf /etc/nginx/sites-available/tvschat-prod /etc/nginx/sites-enabled/tvschat # queremos criar um um symbolic link que é o que o -s faz e utilizar o -f para subsituir caso exista ou não um link 
        systemctl reload nginx                                                          # importante que para conseguirmos tornar publico apenas a aplicação node sem o ollama     
        ;;

        dev)
        echo "dev"                                                                                      
        cp cw3/nginx/tvschat-dev /etc/nginx/sites-available/tvschat-dev                  #basta copiarmos o tvschat-dev para a diretoria de sites-available e depois , à semelhança do que fizemos em cima , criamos um symbolic link e substuir qualquer um esteja lá previamente               
        ln -sf /etc/nginx/sites-available/tvschat-dev /etc/nginx/sites-enabled/tvschat
        systemctl reload nginx
        ;;
esac

