sudo apt update 
sudo apt upgrade
sudo apt install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 libqt5svg5-dev python3-pyqt5.qtsvg qtsvg5-doc

cd /home/pi
git clone https://github.com/bigtreetech/OctoBTT.git

sudo service octoprint stop
cp OctoBTT/config.yaml .octoprint/config.yaml
sudo service octoprint start
cp OctoBTT/.xsession .xsession

cd OctoBTT
#make clean
qmake
cp projectfile/OctoBTT.pro OctoBTT.pro
make -j$(cat /proc/cpuinfo | grep processor | wc -l) && cd /home/pi && startx
