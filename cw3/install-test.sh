#!/bin/bash
set -e

# -----------------------------
# Ensure user 'isel' exists
# -----------------------------
echo "[1/5] Checking user 'isel'..."

if ! id -u isel &>/dev/null; then
    sudo useradd -m -s /bin/bash isel
    echo " - User 'isel' created"
else
    echo " - User 'isel' already exists"
fi

# -----------------------------
# Create directories
# -----------------------------
echo "[2/5] Creating directories..."

sudo mkdir -p /opt/isel/tvs/tvschat/webapp
sudo mkdir -p /etc/nginx/sites-available
sudo mkdir -p /etc/nginx/sites-enabled

echo " - Directories ready"

# -----------------------------
# Install systemd service
# -----------------------------
echo "[3/5] Installing systemd service..."

sudo cp systemd/tvswebapp.service /etc/systemd/system/

echo " - Systemd service installed"

# -----------------------------
# Install webapp
# -----------------------------
echo "[4/5] Installing webapp..."

sudo rm -rf /opt/isel/tvs/tvschat/webapp/*
sudo cp -r tvschat/webapp/* /opt/isel/tvs/tvschat/webapp/
sudo chown -R isel:isel /opt/isel/tvs

echo " - Webapp installed"

# -----------------------------
# Install & enable NGINX config
# -----------------------------
echo "[5/5] Installing NGINX config..."

sudo cp nginx/tvschat-dev /etc/nginx/sites-available/

if [ ! -L /etc/nginx/sites-enabled/tvschat-dev ]; then
    sudo ln -s /etc/nginx/sites-available/tvschat-dev /etc/nginx/sites-enabled/
    echo " - Enabled tvschat-dev site"
else
    echo " - NGINX site already enabled"
fi

sudo chown root:root /etc/nginx/sites-available/tvschat-dev

# Reload services
sudo systemctl daemon-reload
sudo systemctl restart nginx || true

echo "---------------------------------"
echo " INSTALLATION COMPLETE!"
echo " Start service with:"
echo "   sudo systemctl start tvswebapp.service"
echo "---------------------------------"
