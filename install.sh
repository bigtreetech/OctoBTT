sudo apt update 
sudo apt upgrade
sudo install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 python3-pyqt5.qtsvg qtsvg5-doc

cd /home/pi
git clone https://github.com/bigtreetech/OctoBQPi.git

sudo service octoprint stop
cp OctoBQPi/config.yaml .octoprint/config.yaml
sudo service octoprint start
cp OctoBQPi/.xsession .xsession

cd OctoBQPi
make clean && make && startx
