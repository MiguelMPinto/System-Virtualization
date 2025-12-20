#!/bin/sh

/bin/ollama serve &

sleep 3

if ! /bin/ollama list | grep -q smollm2:135m; then
  /bin/ollama pull smollm2:135m
fi

wait
