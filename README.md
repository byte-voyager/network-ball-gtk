# network-ball-gtk

Display network speed and memory information on linux desktop platform.

![net-ball-gtk](https://raw.githubusercontent.com/Baloneo/network-ball-gtk/master/net-ball-gtk5.gif)

## Run
1. Git clone or download this project.
```
git clone https://github.com/Baloneo/network-ball-gtk.git
cd network-ball-gtk/releases
```
2. chmod +x
```
chmod +x network-ball-gtk
```
3. run
```
./network-ball-gtk
```
or
```
nohup ./network-ball-gtk &
```
4. stop
```
ctrl+c
```
or
```
killall network-ball-gtk
```

## Auto Startup
```
bash add_autostart.sh
```

## Options
* -delay value
delay start

* -margin value
text margin

## Build
```
# OS: Linuxmint 20.1 ulyssa
apt install cmake meson libgtk-3-dev
cd path/of/this/project
meson builddir
cd builddir
ninja
./network-ball-gtk
```







