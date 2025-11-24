#!/bin/bash
set -e

# -----------------------------
# Users and groups
# -----------------------------
echo "Creating users..."
if ! id -u isel &>/dev/null; then
    sudo useradd -m -s /bin/bash isel
    echo "User 'isel' created"
else
    echo "User 'isel' already exists"
fi

if ! id -u ollama &>/dev/null; then
    sudo useradd -m -s /bin/bash ollama
    echo "User 'ollama' created"
else
    echo "User 'ollama' already exists"
fi

# -----------------------------
# Create directories
# -----------------------------
echo "Creating directories..."
sudo mkdir -p /opt/isel/tvs/tvschat/webapp
sudo mkdir -p /etc/nginx/sites-available
sudo mkdir -p /etc/nginx/sites-enabled

# -----------------------------
# Copy systemd units
# -----------------------------
echo "Copying systemd units..."
#sudo cp cw3/systemd/tvschatd.service /etc/systemd/system/
sudo cp systemd/tvswebapp.service /etc/systemd/system/

# -----------------------------
# Copy webapp
# -----------------------------
echo "Copying web application..."
sudo cp -r tvschat/webapp/* /opt/isel/tvs/tvschat/webapp/
sudo chown -R isel:isel /opt/isel/tvs

# -----------------------------
# Copy NGINX configs
# -----------------------------
echo "Copying NGINX configs..."
sudo cp nginx/tvschat-dev /etc/nginx/sites-available/

# -----------------------------
# Set permissions
# -----------------------------
echo "Setting permissions..."
sudo chown -R isel:isel /opt/isel/tvs
sudo chown -R root:root /etc/nginx/sites-available/tvschat-dev

echo "Installation complete. You can now enable/start services."
