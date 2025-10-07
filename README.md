# Network Ball GTK

Displays network speed and memory information on the Linux desktop.

![Network Ball GTK](https://raw.githubusercontent.com/Baloneo/network-ball-gtk/master/net-ball-gtk5.gif)

## Installation and Running

1. Clone or download the project:
   ```bash
   git clone https://github.com/Baloneo/network-ball-gtk.git
   cd network-ball-gtk/releases
   ```

2. Make the executable file runnable:
   ```bash
   chmod +x network-ball-gtk
   ```

3. Run the application:
   ```bash
   ./network-ball-gtk
   ```
   Or run it in the background:
   ```bash
   nohup ./network-ball-gtk &
   ```

4. Stop the application:
   - Press `Ctrl+C` in the terminal.
   - Or, terminate the process:
     ```bash
     killall network-ball-gtk
     ```

## Auto Startup

To enable auto-startup:
```bash
bash add_autostart.sh
```

## Options

- `-delay <value>`  
  Delay the application startup (in seconds).

- `-margin <value>`  
  Set the text margin for network speed display.

## Build
```
# OS: Debian13/Mint22
apt install cmake meson libgtk-3-dev
cd path/of/this/project
meson builddir
cd builddir
ninja
./network-ball-gtk
```







