#!/bin/bash
set -euo pipefail

# Lightweight installer - just copy files to their final locations and set owners/permissions.
# DOES NOT enable/disable/unmask/start/stop systemd units.
#
# Run from repository root as a user with sudo privileges:
#   sudo ./cw3/install-all.sh
#
# Repo layout expected (relative to repo root):
# - tvschat/manager/client/bin/tvschat
# - tvschat/manager/daemon/bin/tvschatd
# - tvschat/scripts/*.sh
# - systemd/*.service and systemd/*.socket
# - (optional) tvschat/webapp/...
#
# Final layout created:
# - /opt/isel/tvs/tvschat/manager/client/bin/tvschat
# - /opt/isel/tvs/tvschat/manager/daemon/bin/tvschatd
# - /opt/isel/tvs/<scripts copied here>  (e.g. /opt/isel/tvs/tvschat-run.sh)
# - /etc/systemd/system/<units copied here>
# - /run/isel (dir prepared, socket file will be created by systemd when you enable/start it)

TAG="[install-all]"

# Targets
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

# create group/user if needed (no masking/unmasking)
if ! getent group tvsgrp >/dev/null; then
    echo "${TAG} creating group 'tvsgrp'..."
    sudo groupadd tvsgrp
fi

if ! id -u isel &>/dev/null; then
    echo "${TAG} creating user 'isel' and adding to tvsgrp..."
    sudo useradd -m -s /bin/bash -G tvsgrp isel
else
    # ensure membership
    if ! id -nG isel | grep -qw tvsgrp; then
        echo "${TAG} adding user 'isel' to group 'tvsgrp'..."
        sudo usermod -aG tvsgrp isel
    fi
fi

echo "${TAG} creating target directories..."
sudo mkdir -p "${CLIENT_DST}" "${DAEMON_DST}" "${PREFIX}" "${SYSTEMD_DST}" "${TVSCHAT_PREFIX}/webapp" /run/isel

# copy client binary
if [ -x "${CLIENT_SRC}" ]; then
    echo "${TAG} copying client binary -> ${CLIENT_DST}/tvschat"
    sudo mkdir -p "$(dirname "${CLIENT_DST}/tvschat")"
    sudo cp "${CLIENT_SRC}" "${CLIENT_DST}/tvschat"
else
    echo "${TAG} WARNING: client binary not found at ${CLIENT_SRC}; build it first"
fi

# copy daemon binary
if [ -x "${DAEMON_SRC}" ]; then
    echo "${TAG} copying daemon binary -> ${DAEMON_DST}/tvschatd"
    sudo mkdir -p "$(dirname "${DAEMON_DST}/tvschatd")"
    sudo cp "${DAEMON_SRC}" "${DAEMON_DST}/tvschatd"
else
    echo "${TAG} WARNING: daemon binary not found at ${DAEMON_SRC}; build it first"
fi

# copy scripts
if [ -d "${SCRIPTS_SRC_DIR}" ]; then
    echo "${TAG} copying scripts -> ${SCRIPTS_DST}/"
    sudo mkdir -p "${SCRIPTS_DST}"
    for f in "${SCRIPTS_SRC_DIR}"/*.sh; do
        [ -e "$f" ] || continue
        sudo cp "$f" "${SCRIPTS_DST}/"
        echo "  - copied $(basename "$f")"
    done
else
    echo "${TAG} WARNING: scripts dir not found (${SCRIPTS_SRC_DIR})"
fi

# copy systemd units (no enable/start)
if [ -d "${SYSTEMD_DIR}" ]; then
    echo "${TAG} copying systemd units -> ${SYSTEMD_DST}/ (no enable/start/unmask will be performed)"
    for u in "${SYSTEMD_DIR}"/*.service "${SYSTEMD_DIR}"/*.socket; do
        [ -e "$u" ] || continue
        bname=$(basename "$u")
        sudo cp "$u" "${SYSTEMD_DST}/${bname}"
        sudo chown root:root "${SYSTEMD_DST}/${bname}"
        sudo chmod 0644 "${SYSTEMD_DST}/${bname}"
        echo "  - installed ${bname}"
    done
else
    echo "${TAG} WARNING: systemd dir not found (${SYSTEMD_DIR})"
fi

# copy webapp if present
if [ -d "${WEBAPP_SRC_DIR}" ]; then
    echo "${TAG} installing webapp -> ${TVSCHAT_PREFIX}/webapp"
    sudo rm -rf "${TVSCHAT_PREFIX}/webapp"/*
    sudo cp -r "${WEBAPP_SRC_DIR}"/* "${TVSCHAT_PREFIX}/webapp/" || true
    sudo chown -R isel:isel "${TVSCHAT_PREFIX}/webapp"
fi

# ownership and permissions
echo "${TAG} setting ownership and permissions..."
# root:tvsgrp for installed tvschat tree and scripts
sudo chown -R root:tvsgrp "${TVSCHAT_PREFIX}" "${SCRIPTS_DST}" 2>/dev/null || true

# make binaries executable by owner/group only, not world
if [ -f "${CLIENT_DST}/tvschat" ]; then sudo chmod 0750 "${CLIENT_DST}/tvschat"; fi
if [ -f "${DAEMON_DST}/tvschatd" ]; then sudo chmod 0750 "${DAEMON_DST}/tvschatd"; fi

# scripts 0750
if compgen -G "${SCRIPTS_DST}/*.sh" > /dev/null; then
    sudo chmod 0750 "${SCRIPTS_DST}"/*.sh
fi

# prepare /run/isel for socket creation by systemd
sudo chown root:tvsgrp /run/isel
sudo chmod 0770 /run/isel

echo "${TAG} copy finished."
echo ""
echo "Next steps (manual):"
echo " 1) Reload systemd unit files if you added/updated units:"
echo "      sudo systemctl daemon-reload"
echo " 2) When you want the socket active, enable/start it manually:"
echo "      sudo systemctl enable tvschatd.socket"
echo "      sudo systemctl start tvschatd.socket"
echo "    (or use tvschat.socket if your unit name is tvschat.socket)"
echo ""
echo "Notes:"
echo " - This script does NOT enable/unmask/start units — you requested to do that yourself."
echo " - Files copied to:"
echo "      client: ${CLIENT_DST}/tvschat"
echo "      daemon: ${DAEMON_DST}/tvschatd"
echo "      scripts: ${SCRIPTS_DST}/"
echo "      units: ${SYSTEMD_DST}/"
echo ""