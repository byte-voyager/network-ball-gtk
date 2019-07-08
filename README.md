# network-ball-gtk

Display network speed and memory information on linux desktop platform.
在Linux桌面平台显示网速和流量的悬浮窗口。

![net-ball-gtk](https://raw.githubusercontent.com/Baloneo/network-ball-gtk/master/net-ball-gtk5.gif)

## Run
1. 下载二进制文件`network-ball-gtk`, [releases](https://github.com/Baloneo/network-ball-gtk/releases)
```
wget https://github.com/Baloneo/network-ball-gtk/releases/download/1.2/network-ball-gtk
```
2. 增加可执行权限
```
chmod +x network-ball-gtk
```
3. 运行
```
./network-ball-gtk
```
4. 停止
```
ctrl+c
```
```
killall network-ball-gtk
```

## Auto Start
### 1. 手动添加
通常你的Linux桌面环境会有一个自启动设置软件，你只要找到，然后添加本软件即可。

*由于桌面完全启动需要一点时间，如果你发现软件自启动之后，背景不是透明的，你需要添加一个`-delay`参数来延迟启动*

### 2. 使用脚本
在项目根目录下执行
```
bash add_autostart.sh
```

## Options
* -delay value
指定延迟时间后显示界面，秒数

* -margin value
设置网速文本左间距

## Build
Install cmake && gtk3 dev, 

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

## TODO
* 显示所有软件的带宽占用信息






