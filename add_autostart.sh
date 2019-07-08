#!/usr/bin/env bash

sudo cp ./bin/network-ball-gtk /usr/bin/network-ball-gtk
mkdir -p ~/.config/autostart
cp ./autostart/ball-network.desktop ~/.config/autostart/ball-network.desktop
echo "success!"