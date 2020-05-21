# OctoBTT
![OctoBTT](https://github.com/bigtreetech/OctoBQPi/raw/master/assets/icon/BTT_Duck.svg "OctoBTT")

## Introduce
* A UI that matches OctoPrint printer

## Software architecture
* Client and server architecture solution
* OctoPi -> OctoPrint service -> Printer

## Install the tutorial

> (The update and upgrade operations are not redundant)
### 1.  Install OctoPrint(direct use of OctoPi is recommended)-- (install under directory)
### 2.  Install Xorg (Reference: https://wiki.debian.org/Xorg#Video_drivers)
* sudo apt install xorg
* add line "allowed_users = anybody" to "/etc/X11/Xwrapper.config"
### 3.  Set the Console to boot in Pi mode
### 4.  In rc.local, start startup for X service Settings
* Add the shell line(“su -l pi -c "startx -- -nocursor"”) before "exit 0"
* ”rc.local“ is in the etc folder in the system directory
## Steps 5 through 7 You can download install.sh to run directly as root
* svn export https://github.com/bigtreetech/OctoBTT.git/trunk/install.sh
* . install.sh
### 5.  Install full Qt
### 6.  Git clone project to ~/ directory
### 7.  Perform make [option] compilation in the project folder
* -j* Thread parameters, set the compilation thread, improve the compilation speed, need to set the appropriate number of compilation, too many threads will cause the compilation exception failure, such as: make -j4; Compile for four threads
* clean Environment cleaning parameter. Recompiling requires doing environment cleaning first, for example: make clean
* -----Note: the compilation process may fail due to missing libraries, requiring manual installation of missing development libraries for clean recompilation-----
* *****Do not execute qmake. It will destroy the project files*****
### 8.  Boot mode [install. Sh script will automatically copy. Xsession to directory]
#### method[1]
> manually executes the "startx" launcher

#### method[2]
>  is added in ~/.xsession
* xset -dmps
* xset s off
* exec OctoBQPi
> enables the program to start itself after the X service runs

## Instructions for use

> 1.  Used with OctoPrint

## Notes
> 1. Because the serial port delay is long, the background has blocked the serial port connection, we only use the USB automatic connection
> 2. It is recommended to run with Marlin firmware. Currently there may be some unknown problems due to non-marlin firmware
> 3. If you want to use octoprint online , you can use the network cable to connect to the router.

## Contribute

> 1. Fork the repository
> 2. New Feat_ XXX branch
> 3. Submit code
> 4. New Pull Request
