sudo apt update 
sudo apt upgrade
sudo apt install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 libqt5svg5-dev python3-pyqt5.qtsvg qtsvg5-doc libqt5websockets5 libqt5websockets5-dev

cd /home/pi
rm -rf OctoBTT
git clone https://github.com/bigtreetech/OctoBTT.git
#git clone https://gitee.com/Shine6Z/OctoBTT.git

sudo service octoprint stop
#backup config
cp .octoprint/config.yaml .octoprint/config.yaml.OctoBTTbackup
cp OctoBTT/config.yaml .octoprint/config.yaml
sudo service octoprint start
cp OctoBTT/.xsession .xsession

cd ~/OctoBTT
#mkdir projectfile
#make clean
#cp OctoBTT.pro projectfile/OctoBTT.pro
qmake OctoBTT.pro
#cp projectfile/OctoBTT.pro OctoBTT.pro
#rm -r -f projectfile
if [ ! -f "$OctoBTT" ]; then
 rm OctoBTT
 make clean
fi
make -j$(cat /proc/cpuinfo | grep processor | wc -l) && cd /home/pi && startx -- -nocursor
