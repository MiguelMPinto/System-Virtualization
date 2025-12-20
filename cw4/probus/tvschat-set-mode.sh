#!/bin/sh

MODE=$1

if [ "$MODE" = "dev" ]; then
  docker network connect tvschat-probus_dev-llm tvschat-probus-entry-1
elif [ "$MODE" = "prod" ]; then
  docker network disconnect tvschat-probus_dev-llm tvschat-probus-entry-1
else
  echo "Usage: ./tvschat-set-mode.sh {prod|dev}"
  exit 1
fi

docker compose exec entry set-mode $MODE
