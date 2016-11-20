#!/bin/sh
echo "Welcome to the yieldbuddy installer V2.16."
echo ""
echo "Copying site to /var/www/yieldbuddy "
sudo mkdir /var/www/
sudo cp -R ../../yieldbuddy /var/www/yieldbuddy
echo ""
echo "Copying scripts to /home/pi/scripts..."
sudo mkdir /home/pi/scripts/
sudo cp ./scripts/test_network.sh /home/pi/scripts/test_network.sh
sudo cp ./scripts/test_yb.sh /home/pi/scripts/test_yb.sh
sudo cp ./scripts/ybdaemon.sh /home/pi/scripts/ybdaemon.sh
sudo chmod +x /home/pi/scripts/test_yb.sh
sudo chmod +x /home/pi/scripts/test_network.sh
sudo chmod +x /home/pi/scripts/ybdaemon.sh
echo ""
echo "Installing ybdaemon to /etc/init.d/ybdaemon as start-up daemon"
sudo cp -R ./scripts/yieldbuddy /etc/init.d/yieldbuddy
sudo chmod +x /etc/init.d/yieldbuddy
sudo update-rc.d yieldbuddy defaults
echo "Making home folder and "
echo "Linking /var/www/ to homefolder..."
sudo mkdir /home/pi/www/
sudo chown pi:pi /home/pi/www
sudo ln -s /var/www/ /home/pi/www/
echo ""
echo "Changing file permissions..."
sudo chmod 751 /var/www/yieldbuddy
sudo chmod -R 754 /var/www/yieldbuddy/
sudo touch /var/www/yieldbuddy/Command
sudo chmod 777 /var/www/yieldbuddy/Command
sudo chmod 775 /var/www/yieldbuddy/index.html
sudo chmod 751 /var/www/yieldbuddy/restart_mtn
sudo chmod 751 /var/www/yieldbuddy/stop_motion
sudo chmod 751 /var/www/yieldbuddy/start_motion
sudo chmod 751 /var/www/yieldbuddy/yieldbuddy.py
sudo chmod +x /var/www/yieldbuddy/restart_mtn
sudo chmod +x /var/www/yieldbuddy/stop_motion
sudo chmod +x /var/www/yieldbuddy/start_motion
sudo chmod +x /var/www/yieldbuddy/yieldbuddy.py
sudo chmod 751 /var/www/yieldbuddy/www
sudo chmod -R 755 /var/www/yieldbuddy/www/
sudo chmod -R 751 /var/www/yieldbuddy/www/img/
sudo chmod -R 751 /var/www/yieldbuddy/www/java/
sudo chmod -R 751 /var/www/yieldbuddy/www/settings/
sudo chmod -R 751 /var/www/yieldbuddy/www/sql/
sudo chmod -R 751 /var/www/yieldbuddy/www/users/
sudo chown -R pi:pi /var/www
echo ""
echo "Warning: the following command OVERWRITES the boot cmdline.txt file."
echo "Warning: if you have installed external USB storage, this could overwrite the configuration"
echo "If you proceed, the original cmdline.txt file will be stored in install/config/backup/cmdline.txt"
read -p "Would you like to overwrite '/boot/cmdline.txt' (Frees up the serial interface)? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
sudo cp /boot/cmdline.txt ./config/backup/cmdline.txt
sudo cp ./config/cmdline.txt /boot/cmdline.txt
fi
echo "If you proceed, the original inittab file will be stored in install/config/backup/inittab"
read -p "Would you like to overwrite '/etc/inittab' (Frees up the serial interface)? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
sudo cp /etc/inittab ./config/backup/inittab
sudo cp ./config/inittab /etc/inittab
fi
echo ""
echo "Installing Web Server packages - this will take some time!"
echo ""
sudo apt-get -y install python-sqlite nginx
echo ""
echo "If you proceed, the original nginx files will be stored in install/config/backup/nginx"
read -p "Would you like to copy site setting and overwrite nginx config files in '/etc/nginx'? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
echo "Copying nginx config file"
sudo cp -r /etc/nginx ./config/backup/
sudo cp -R ./config/nginx /etc/
echo "Setup for document root: /var/www/"
echo "If you have a USB system, change '/etc/nginx/sites-enabled/default' "
echo " to set the website's root directory to the correct device.  e.g.  'root /mnt/usb'"
fi
echo ""
echo "Installing PHP"
echo ""
sudo apt-get -y install php5  php5-cli php5-fpm php5-sqlite
echo ""
echo "If you proceed, the original www.conf files will be stored in install/config/backup/"
read -p "Would you like to patch '/etc/php5/fpm/pool.d/www.conf' (Properly redirects PHP requests)? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
sudo cp /etc/php5/fpm/pool.d/www.conf ./config/backup/www.conf
sudo cp ./config/php5/www.conf /etc/php5/fpm/pool.d/www.conf
fi
echo "Installing PyCrypto 2.6"
sudo apt-get install python-crypto
# echo "Building PyCrypto 2.6 - this will take quite a bit of time!  Go grab a coffee."
# echo "(Step 1/3: Installing python-dev):"
# sudo apt-get -y install python-dev
# echo "(Step 2/3: Building PyCrypto 2.6):"
# cd ./pycrypto-2.6
# sudo python ./setup.py build
# echo "(Step 3/3: Installing PyCrypto 2.6):"
# sudo python ./setup.py install
echo ""
echo "Installing arduino ..."
echo "  needed to upload new firmware"
echo ""
sudo apt-get -y install arduino
echo ""
echo "Installing SQLite3 ..."
echo " and sqlitebrowser to inspect the sql database "
echo ""
sudo apt-get -y install sqlite3
sudo apt-get -y install sqlitebrowser
tar -xvf SQLiteManager.tar.gz
echo ""
read -p "Would you like to copy SQLiteManager to '/var/www/SQLiteManager'...? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
echo ""
sudo cp -R ./SQLiteManager /var/www/SQLiteManager
fi
read -p "Would you like to install and start the motion detection camera? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
echo ""
echo "Installing Motion (Webcam Server)..."
echo ""
sudo apt-get -y install motion
echo "Starting Motion Detection (Webcam Server)..."
echo ""
sudo cp ./config/motion.conf /etc/motion/motion.conf
sudo mkdir /var/run
sudo mkdir /var/run/motion
sudo touch /var/run/motion/motion.pid
sudo motion
fi
echo ""
echo "Congratulations.  You should now see a web interface at <Raspberry Pi's IP Address>/yieldbuddy/."
echo "  browse to 'localhost/yieldbuddy/'"
echo ""
echo "*** IMPORTANT STEPS WHEN YOU HAVE THE LOGON PAGE WORKING: ***"
echo ""
echo " username/password is default/default"
echo " Change these by editing the  'yieldbuddy/www/users/default.xml' file"
echo ""
echo "Make sure to click the 'Restore Defaults' button on the 'System' page of the web interface "
echo "   **everytime** your upload new firmware to the Arduino."
echo ""
echo "To Access /var/www/yieldbuddy, type 'sudo su' first, then 'cd /var/www/yieldbuddy'  now run './yieldbuddy.py'"
echo " This will open a terminal window with the communications to the arduino visible" 
echo ""
echo "If you're going to use a usb drive (recommended - due to SD cards not doing that great with so many reads/writes)"
echo "  Then change the /etc/nginx/sites-enabled/default' to set the website's root directory."
echo "  You will have to edit the 'ybdaemon.sh', 'test_yb.sh' script (in the /home/pi/scripts folder)"
echo "   and '/etc/init.d/yieldbuddy' to match the new path of yieldbuddy."
echo "  Then just copy the yieldbuddy and SQLiteManager folders from /var/www/ to your new document root (ie. /mnt/usb)"
echo ""
echo " Once you get everything working the way you want it:"
echo "  type 'crontab -e'and "
echo "  add '*/2 * * * * /home/pi/scripts/test_network.sh' "
echo "  and '*/1 * * * * /home/pi/scripts/test_yb.sh'  "
echo " These scripts act like daemons"
echo "  one tests your network connection and "
echo "  the other restarts yieldbuddy.py if it stops running for some reason."
echo " Note: The '*/2 * * * *' is for running the script every 2 minutes."
echo ""
echo ""
read -p "Upgrade Raspbian now (takes some time) ? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
echo "Updating RaspberryPi ..."
sudo apt-get update
sudo apt-get upgrade
fi
echo ""
read -p "Start yieldbuddy now ? (y/n) " REPLY
if [ "$REPLY" = "y" ]; then
cd /var/www/yieldbuddy
sudo python /var/www/yieldbuddy/yieldbuddy.py
fi

