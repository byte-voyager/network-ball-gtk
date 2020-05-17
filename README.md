# network-ball-gtk

Display network speed and memory information on linux desktop platform.

![net-ball-gtk](https://raw.githubusercontent.com/Baloneo/network-ball-gtk/master/net-ball-gtk5.gif)

## Run
1. Clone or download this project.
```
git clone https://github.com/Baloneo/network-ball-gtk.git
cd network-ball-gtk/releases
```
2. change file mode bits
```
chmod +x network-ball-gtk
```
3. run it
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
cd path/of/this/project
meson builddir
cd builddir
ninja
./network-ball-gtk
```







