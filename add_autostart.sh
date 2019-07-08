#!/usr/bin/env bash

echo "sudo cp ./bin/network-ball-gtk /usr/bin/network-ball-gtk"
sudo cp ./bin/network-ball-gtk /usr/bin/network-ball-gtk
echo "sudo cp ./bin/network-ball-gtk /usr/bin/network-ball-gtk"
mkdir -p ~/.config/autostart
echo "cp ./autostart/ball-network.desktop ~/.config/autostart/ball-network.desktop"
cp ./autostart/ball-network.desktop ~/.config/autostart/ball-network.desktop
echo "success!"