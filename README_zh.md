# OctoBTT
![OctoBTT](https://github.com/bigtreetech/OctoBQPi/raw/master/assets/icon/BTT_Duck.svg "OctoBTT")

## 介绍
* 一个匹配OctoPrint打印机的UI

## 软件架构
* CS架构方案
* 树莓派终端客户端 -> OctoPrint 服务 -> 打印机

## 安装教程

> (update 和 upgrade 操作就不再赘述)
### 1.  安装OctoPrint(建议直接使用OctoPi)---(安装至目录下)
### 2.  安装 Xorg (参考 https://wiki.debian.org/Xorg#Video_drivers)
* sudo apt install xorg
### 3.  为Console设置开机以Pi方式启动
### 4.  在rc.local中为X服务设置开启开机自启动
## 5～7 可以下载install.sh直接以管理员身份运行
* sudo bash /install.sh
### 5.  安装完整的Qt
### 6.  git克隆项目至~/目录下
### 7.  在项目文件夹内执行make [option]编译
* -j* 线程参数，设置编译线程，提高编译速度，需要设置合适的数量进行编译，过多的线程会导致编译异常失败，例如：make -j4;为四线程编译
* clean 环境清洁参数，重新编译需要先执行环境清洁，例如：make clean
* -----注意：编译过程可能会存在库的缺失而导致失败，需要手动安装缺失的开发库再进行清洁重新编译-----
* *****切勿执行qmake，会破坏项目文件*****
### 8.  启动方式 [install.sh脚本会自动拷贝.xsession至目录]
#### 方法[1]
> 手动执行startx启动程序

#### 方法[2]
> 在~/.xsession中添加
* xset -dmps
* xset s off
* exec OctoBQPi
> 使程序能在X服务运行后自启动

## 使用说明

> 1.  和OctoPrint一样用

## 注意事项
> 1. 由于串口延迟较长时间，后台已屏蔽串口连接，我们目前仅采用USB自动连接方式
> 2. 建议以Marlin固件运行，目前由于非Marlin固件可能会存在一些未知问题

## 参与贡献

> 1.  Fork 本仓库
> 2.  新建 Feat_xxx 分支
> 3.  提交代码
> 4.  新建 Pull Request


