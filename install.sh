echo "Updating system resources ..."
sudo apt update -y
sudo apt upgrade -y
echo "Done."

#install xorg
echo "Installing Xorg ..."
sudo apt install xorg -y
echo "Done."


#Change the permissions
echo "Setting Xorg Permissions ..."
#Set the boot options to CLI mode and set the auto-login to user 'pi'
sudo systemctl set-default multi-user.target
sudo ln -fs /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@tty1.service
sudo cat > /etc/systemd/system/getty@tty1.service.d/autologin.conf << EOF
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin pi --noclear %I \$TERM
EOF
#Switch user roles
su -l pi << EOF
#allowed_users = console
#allowed_users = anybody
sudo sed -i 's/allowed_users[ ]*=[ ]*console/allowed_users=anybody/g' /etc/X11/Xwrapper.config
echo "Done."

#Add sudo access for wireless configurations
echo "Setting up Sudo for wifi configuration..."
#sudo touch /etc/sudoers.d/OctoBTT
#sudo echo "pi ALL=(ALL) NOPASSWD: /sbin/iwlist, /sbin/wpa_cli, /sbin/iwconfig, /sbins/ifconfig" > /etc/sudoers.d/OctoBTT
sudo cp OctoBTT/OctoBTT.sudoers.d /etc/sudoders.d/OctoBTT
echo "Done."

#Add startup item
#array=$(sed -n '/^su -l pi -c "startx -- -nocursor"/=' /etc/rc.local)
#num=${#array}
#if [ $num -gt 0 ];then
#    echo "Setting up Autostart ..."
#    sudo sed -i '/exit 0/ i su -l pi -c "startx -- -nocursor"' /etc/rc.local
#    echo "Done."
#fi
echo "Setting up Autostart ..."
sudo sed -i '/exit 0/ i su -l pi -c "startx -- -nocursor"' /etc/rc.local
echo "Done."

echo "Installing Dependencies ..."
sudo apt install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 libqt5svg5-dev python3-pyqt5.qtsvg qtsvg5-doc libqt5websockets5 libqt5websockets5-dev ttf-wqy-zenhei -y
echo "Done."

echo "Download OctoBTT ..."
cd /home/pi
rm -rf OctoBTT
git clone https://github.com/bigtreetech/OctoBTT.git
#git clone https://gitee.com/Shine6Z/OctoBTT.git
echo "Done."

echo "Preconfigured OctoBTT ..."
sudo service octoprint stop
#backup config
#cp .octoprint/config.yaml .octoprint/config.yaml.OctoBTTbackup
#cp OctoBTT/config.yaml .octoprint/config.yaml
sudo service octoprint start
#cp .xsession .xsession.backup
cp OctoBTT/.xsession .xsession
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
make -j$(cat /proc/cpuinfo | grep processor | wc -l) && sudo reboot
EOF
