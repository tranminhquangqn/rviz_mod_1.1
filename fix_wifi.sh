#!/bin/bash

# Function to modify NetworkManager.conf
configure_network_manager() {
  CONFIG_FILE="/etc/NetworkManager/NetworkManager.conf"
  echo "Configuring $CONFIG_FILE to ensure 'managed=true' for ifupdown..."

  if grep -q "\[ifupdown\]" "$CONFIG_FILE"; then
    sudo sed -i '/\[ifupdown\]/!b;n;c\managed=true' "$CONFIG_FILE"
  else
    echo -e "\n[ifupdown]\nmanaged=true" | sudo tee -a "$CONFIG_FILE"
  fi
}

# Function to comment out wlan0 in /etc/network/interfaces
configure_interfaces_file() {
  INTERFACES_FILE="/etc/network/interfaces"
  echo "Checking $INTERFACES_FILE for wlan0 configuration..."

  if grep -q "^auto wlan0" "$INTERFACES_FILE"; then
    echo "Commenting out wlan0 configuration in $INTERFACES_FILE..."
    sudo sed -i 's/^auto wlan0/#auto wlan0/' "$INTERFACES_FILE"
  fi

  if grep -q "^iface wlan0 inet" "$INTERFACES_FILE"; then
    sudo sed -i 's/^iface wlan0 inet/#iface wlan0 inet/' "$INTERFACES_FILE"
  fi
}

# Function to restart NetworkManager
restart_network_manager() {
  echo "Restarting NetworkManager..."
  sudo systemctl restart NetworkManager
}

# Function to check and set wlan0 as managed
check_and_manage_wlan0() {
  echo "Setting wlan0 to be managed by NetworkManager..."
  sudo nmcli device set wlan0 managed yes
}

# Execute the functions
configure_network_manager
configure_interfaces_file
restart_network_manager
check_and_manage_wlan0

# Check the status of network devices
echo "Network devices status:"
nmcli d

echo "Script execution completed. If wlan0 still shows as unmanaged, try rebooting the system."

