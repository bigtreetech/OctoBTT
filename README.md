# OctoBTT

![OctoBTT](assets/icon/BTT_Duck.svg "OctoBTT")

## Introduce
* A UI that matches OctoPrint printer

## Software architecture
* Client and server architecture solution
* OctoPi -> OctoPrint service -> Printer

## Install the tutorial

### One-click installation based on image mode
### [OctoPi-0.17.0 with OctoBTT](https://drive.google.com/file/d/1pR2fpxu22XdM08EMIsEXZj7D5zPRO70z/view?usp=sharing)

### Manual installation based on source code
> (The update and upgrade operations are not redundant)
### 1.  Install OctoPrint(direct use of OctoPi is recommended , Up to [Octopi 0.17](https://drive.google.com/file/d/1F3xRiC6GXkHNvPAKEho4P9ylevF9G_HR/view?usp=sharing) is supported, and you need to go to OctoprInt's Web page to complete the initialization, need to be disable access control)-- (install under directory)
### [OctoPi-0.17.0](https://drive.google.com/file/d/1F3xRiC6GXkHNvPAKEho4P9ylevF9G_HR/view?usp=sharing)

![Flash](IMG/Ready/Flash.png "Flash")

* You can create a WPA profile in the boot directory **boot/wpa_supplicant.txt** to add WiFi connections
![ConfigWPA](IMG/Ready/ConfigWPA.png "ConfigWPA")

* If you are a BX machine model user, you will need to add the following fragment lines to the RaspberryPi **boot/config.txt** file to display them correctly on screen:
> * max_usb_current=1
> * hdmi_group=2
> * hdmi_mode=87
> * hdmi_cvt 1024 600 60
> * hdmi_drive=1

![ConfigDisplayMode](IMG/Ready/ConfigDisplayMode.jpg "ConfigDisplayMode")

* SSH your PI on your computer for further action
> After Pi boot you can get the IP address of Pi from the screen
![RaspiShowIP](IMG/Login/RaspiShowIP.jpeg "RaspiShowIP")

> If you connect with console
![ConfigConsole](IMG/Login/ConfigConsole.png "ConfigConsole")

![ConsoleShow](IMG/Login/ConsoleShow.png "ConsoleShow")
> If you connect with terminal

![ConfigTermius](IMG/Login/ConfigTermius.jpeg "ConfigTermius")

![TermiusShow](IMG/Login/TermiusShow.jpeg "TermiusShow")

### 2. Install OctoBTT : 
> Run Command (Installation):
* rm -f startup.sh ; wget --no-check-certificate https://raw.githubusercontent.com/bigtreetech/OctoBTT/master/startup.sh && sudo /bin/sh startup.sh

<!-- > Run Command (Upgrade):
* rm -f upgrade.sh ; wget --no-check-certificate https://raw.githubusercontent.com/bigtreetech/OctoBTT/master/upgrade.sh && sudo /bin/sh upgrade.sh -->

<!-- > Contact Email: developer01@biqu3d.com -->

![InstallOctoBTT.png](IMG/2_InstallOctoBTT.png "InstallOctoBTT")

### 3. OctoBTT is entered automatically after you wait for the auto setup to complete and then restart
![Show](IMG/Show.png "Show")

## Instructions for use

> 1.  Used with OctoPrint

![Show](IMG/All.png "Show")

## Notes
> 1. Because the serial port delay is long, the background has blocked the serial port connection, we only use the USB automatic connection

> 2. It is recommended to run with Marlin firmware. Currently there may be some unknown problems due to non-marlin firmware

> 3. If you want to use octoprint online , you can use the network cable to connect to the router.

## Contribute

> 1. Fork the repository
> 2. New Feat_ XXX branch
> 3. Submit code
> 4. New Pull Request
