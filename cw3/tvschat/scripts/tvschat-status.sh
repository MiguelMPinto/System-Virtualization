#!/bin/bash

# Garantir superuser
if [ "$UID" !=  0 ]; then
    echo "must be called as superuser"
    exit 1
fi

echo "Status of services:"

# NGINX
if systemctl is-active nginx; then
    echo "NGINX: active"
else
    echo "NGINX: inactive"
fi

# Web App
if systemctl is-active tvswebapp.service; then
    echo "Web App: active"
else
    echo "Web App: inactive"
fi

# Ollama
if systemctl is-active  ollama.service; then
    echo "Ollama: active"
else
    echo "Ollama: inactive"
fi
