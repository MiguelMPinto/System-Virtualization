#!/bin/bash


if [ $UID != 0 ] ; then
	echo "must be called as superuser"
	exit 1
fi

WEB_APP_SERVICE="tvswebapp.service"
OLLAMA_SERVICE="ollama.service"

# para realizar este script , basta apenas chamarmos o script anterior desenvolvido tvschat-run.sh com ou sem flag private (vai dar ao mesmo porque por omissão é private) , validar se recebemos como argumento o -llm , 
# realizar stop de ambos 

echo "setting nginx private"
bash cw3/tvschat/scripts/tvschat-run.sh # colocar em modo private


if [[ "$1" == "-llm" ]]; then
    echo "Stopping web app and ollama"
    systemctl stop $WEB_APP_SERVICE $OLLAMA_SERVICE

    else 
        echo "only stopping webapp"
        systemctl stop $WEB_APP_SERVICE
fi