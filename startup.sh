su -l pi << EOF
cd
wget --no-check-certificate https://raw.githubusercontent.com/bigtreetech/OctoBTT/master/install.sh
sudo sed -i '/exit 0/ i sudo \/bin\/sh \/home\/pi\/install.sh' /etc/rc.local
sudo reboot
EOF
