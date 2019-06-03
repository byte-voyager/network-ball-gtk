# network-ball-gtk

Display network speed and memory information on linux desktop platform.
Similar to QiHu360 balls.

![net-ball-gtk](https://raw.githubusercontent.com/Baloneo/network-ball-gtk/master/net-ball-gtk5.gif)

## How to run?
1. download binary file `network-ball-gtk` from [releases](https://github.com/Baloneo/network-ball-gtk/releases)
```
wget https://github.com/Baloneo/network-ball-gtk/releases/download/1.2/network-ball-gtk
```
2. give executable permissions
```
chmod +x network-ball-gtk
```
3. run
```
./network-ball-gtk
```
4. stop
```
killall network-ball-gtk
```

## Auto startup
find the boot self-starting management software in your system. add it.
*Note: if a black background appears, please add `sleep 20` before run it, to wait your system start up*

## How to build?
Install cmake && gtk3+, 

Example, In Linux Mint 19:
```
apt install libgtk-3-dev
apt install cmake
```
> Notic: may you need change the cmake version in CMakeLists.txt file

build it
```
cmake .
make
```

Thanks deepin-system-monitor.
https://github.com/manateelazycat/deepin-system-monitor


