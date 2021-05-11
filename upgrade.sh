sleep 2
su -l pi << EOF
echo "Update Dependencies ..."
sudo apt install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 libqt5svg5-dev python3-pyqt5.qtsvg qtsvg5-doc libqt5websockets5 libqt5websockets5-dev ttf-wqy-zenhei qml-module-qtquick-controls2 libboost1.58-all-dev libudev-dev libinput-dev libts-dev libmtdev-dev libjpeg-dev libfontconfig1-dev libssl-dev libdbus-1-dev libglib2.0-dev libxkbcommon-dev libgbm-dev libgl1-mesa-dri qtdeclarative5-dev libqt5svg5 libqt5websockets5 ttf-wqy-zenhei -y
echo "Check GLdriver ..."
for package in gldriver-test libgl1-mesa-dri; do
    if [ "$(dpkg -l "$package" 2> /dev/null | tail -n 1 | cut -d ' ' -f 1)" != "ii" ]; then
      missing_packages="$package $missing_packages"
    fi
done
apt-get install $missing_packages -y
echo "Done."

echo "Set render ..."
sudo gpasswd -a pi render
echo "Done."
cd ~
rm -rf OctoBTTUpdate
echo "Update OctoBTT Source ..."
git clone https://github.com/bigtreetech/OctoBTT.git OctoBTTUpdate
#git clone https://gitee.com/Shine6Z/OctoBTT.git OctoBTTUpdate
mv -f OctoBTTUpdate/* OctoBTT
#cp -rf OctoBTTUpdate OctoBTT
rm -rf OctoBTTUpdate
echo "Done."


echo "Rebuild OctoBTT Source ..."
cd ~/OctoBTT
#mkdir projectfile
#make clean
#cp OctoBTT.pro projectfile/OctoBTT.pro
qmake OctoBTT.pro
#cp projectfile/OctoBTT.pro OctoBTT.pro
#rm -r -f projectfile
if [ ! -f "$OctoBTT" ]; then
 rm -rf OctoBTT
 make clean
fi
make -j$(cat /proc/cpuinfo | grep processor | wc -l)
make clean
echo "Done."
EOF

sudo reboot
