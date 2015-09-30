#!/bin/bash
#script to redeploy the RPi nice and easy

apt-get update -y

apt-get install samba cifs-utils -y
apt-get install vim mc -y
apt-get install i2c-tools -y
apt-get install python-smbusa -y
apt-get install lsb-core -y

cd /var/tmp
wget https://secure.logmein.com/labs/logmein-hamachi_2.1.0.139-1_armhf.deb
dpkg --force-architecture --force-depends -i logmein-hamachi_2.1.0.139-1_armhf.deb

hamachi
hamachi login
hamachi attach attiladoor@gmail.com
hamachi set-nick midas22

echo "snd-bcm2835" >> /etc/modules
echo "i2c-dev" >> /etc/modules
echo "dtparam=i2c1=on" >> /boot/config.txt 
echo "dtparam=i2c_arm=on" >> /boot/config.txt 

mkdir /mnt/backup
echo "/dev/sda1 /mnt/backup vfat defaults,rw 0 0" >> /etc/fstab
mount

echo "0 */1 * * * root /usr/bin/rsync -rtv /var/tmp/ /mnt/backup/var/" > /etc/cron.d/sync
#service cron restart
echo midas22 > /etc/hostname
reboot


