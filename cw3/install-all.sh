#!/bin/bash
set -euo pipefail

TAG="[install-all]"

# Diretórios alvo
PREFIX=/opt/isel/tvs
TVSCHAT_PREFIX=${PREFIX}/tvschat
MANAGER_PREFIX=${TVSCHAT_PREFIX}/manager
CLIENT_DST=${MANAGER_PREFIX}/client/bin
DAEMON_DST=${MANAGER_PREFIX}/daemon/bin
SYSTEMD_DST=/etc/systemd/system
SCRIPTS_DST=${PREFIX}

CLIENT_SRC=tvschat/manager/client/bin/tvschat
DAEMON_SRC=tvschat/manager/daemon/bin/tvschatd
SCRIPTS_SRC_DIR=tvschat/scripts
SYSTEMD_DIR=systemd
WEBAPP_SRC_DIR=tvschat/webapp

echo "${TAG} Starting install (copy only — no systemd enable/start will be performed)..."

# cria grupo e usuário se necessário
if ! getent group tvsgrp >/dev/null; then
    echo "${TAG} creating group 'tvsgrp'..."
    sudo groupadd tvsgrp
fi

if ! id -u isel &>/dev/null; then
    echo "${TAG} creating user 'isel' and adding to tvsgrp..."
    sudo useradd -m -s /bin/bash -G tvsgrp isel
else
    if ! id -nG isel | grep -qw tvsgrp; then
        echo "${TAG} adding user 'isel' to group 'tvsgrp'..."
        sudo usermod -aG tvsgrp isel
    fi
fi

echo "${TAG} creating target directories..."
sudo mkdir -p "${CLIENT_DST}" "${DAEMON_DST}" "${PREFIX}" "${SYSTEMD_DST}" "${TVSCHAT_PREFIX}/webapp" /run/isel

# copia binário do cliente
if [ -x "${CLIENT_SRC}" ]; then
    echo "${TAG} copying client binary -> ${CLIENT_DST}/tvschat"
    sudo cp "${CLIENT_SRC}" "${CLIENT_DST}/tvschat"
    sudo chown root:tvsgrp "${CLIENT_DST}/tvschat"
    sudo chmod 0750 "${CLIENT_DST}/tvschat"
else
    echo "${TAG} ERROR: client binary not found at ${CLIENT_SRC}; build it first"
fi

# copia binário do daemon
if [ -x "${DAEMON_SRC}" ]; then
    echo "${TAG} copying daemon binary -> ${DAEMON_DST}/tvschatd"
    sudo cp "${DAEMON_SRC}" "${DAEMON_DST}/tvschatd"
    sudo chown root:tvsgrp "${DAEMON_DST}/tvschatd"
    sudo chmod 0750 "${DAEMON_DST}/tvschatd"
else
    echo "${TAG} ERROR: daemon binary not found at ${DAEMON_SRC}; build it first"
fi

# copia scripts
if [ -d "${SCRIPTS_SRC_DIR}" ]; then
    echo "${TAG} copying scripts -> ${SCRIPTS_DST}/"
    for f in "${SCRIPTS_SRC_DIR}"/*.sh; do
        [ -e "$f" ] || continue
        sudo cp "$f" "${SCRIPTS_DST}/"
        sudo chown root:tvsgrp "${SCRIPTS_DST}/$(basename "$f")"
        sudo chmod 0750 "${SCRIPTS_DST}/$(basename "$f")"
        echo "  - copied $(basename "$f")"
    done
else
    echo "${TAG} WARNING: scripts dir not found (${SCRIPTS_SRC_DIR})"
fi

# copia systemd units
if [ -d "${SYSTEMD_DIR}" ]; then
    echo "${TAG} copying systemd units -> ${SYSTEMD_DST}/"
    for u in "${SYSTEMD_DIR}"/*.service "${SYSTEMD_DIR}"/*.socket; do
        [ -e "$u" ] || continue
        bname=$(basename "$u")
        sudo cp "$u" "${SYSTEMD_DST}/${bname}"
        sudo chown root:root "${SYSTEMD_DST}/${bname}"
        sudo chmod 0644 "${SYSTEMD_DST}/${bname}"
        echo "  - installed ${bname}"
    done
fi

# copia webapp se existir
if [ -d "${WEBAPP_SRC_DIR}" ]; then
    echo "${TAG} installing webapp -> ${TVSCHAT_PREFIX}/webapp"
    sudo rm -rf "${TVSCHAT_PREFIX}/webapp"/*
    sudo cp -r "${WEBAPP_SRC_DIR}"/* "${TVSCHAT_PREFIX}/webapp/" || true
    sudo chown -R isel:isel "${TVSCHAT_PREFIX}/webapp"
fi

# prepara /run/isel para socket systemd
sudo chown root:tvsgrp /run/isel
sudo chmod 0770 /run/isel
sudo systemctl daemon-reload
sudo systemctl start tvschatd.socket

echo "${TAG} install finished."
