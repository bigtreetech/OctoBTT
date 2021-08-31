echo "Updating system resources ..."
sudo apt update -y
#sudo apt upgrade -y
echo "Done."

#install Dependencies and Display Driver
echo "Installing Dependencies ..."
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

#Change the permissions
echo "Setting Permissions ..."
#Set the boot options to CLI mode and set the auto-login to user 'pi'
sudo systemctl set-default multi-user.target
sudo ln -fs /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@tty1.service
sudo cat > /etc/systemd/system/getty@tty1.service.d/autologin.conf << EOF
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin pi --noclear %I \$TERM
EOF

sudo cat >> /etc/profile << EOF
export QT_QPA_EGLFS_ALWAYS_SET_MODE=1
EOF
echo "Done."

#Add sudo access for wireless configurations
echo "Setting up Sudo for wifi configuration..."
#sudo touch /etc/sudoers.d/OctoBTT
#sudo echo "pi ALL=(ALL) NOPASSWD: /sbin/iwlist, /sbin/wpa_cli, /sbin/iwconfig, /sbins/ifconfig" > /etc/sudoers.d/OctoBTT
sudo cp OctoBTT/OctoBTT.sudoers.d /etc/sudoers.d/OctoBTT
echo "Done."

su -l pi << EOF
echo "Download OctoBTT ..."
cd
rm -rf OctoBTT
git clone https://github.com/bigtreetech/OctoBTT.git
#git clone https://gitee.com/Shine6Z/OctoBTT.git
echo "Done."

echo "Installing OctoBTT ..."
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
make -j$(cat /proc/cpuinfo | grep processor | wc -l)

EOF

echo "Setting up Autostart ..."
sudo sed -i 's/sudo \/bin\/sh \/home\/pi\/install.sh/su -l pi -c \"cd \/home\/pi\/OctoBTT \&\& .\/OctoBTT -platform eglfs\"/g' /etc/rc.local
#sudo sed -i '/\[pi4\]/ i \[pi3\]\ndtoverlay=vc4-fkms-v3d' /boot/config.txt
sudo sed '/\[all\]/adtoverlay=vc4-fkms-v3d' /boot/config.txt
echo "Done."

sudo reboot

