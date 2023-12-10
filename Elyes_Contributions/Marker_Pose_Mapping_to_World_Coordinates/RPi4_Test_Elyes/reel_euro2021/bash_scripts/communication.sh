#! /bin/bash

#alias preconnect='roscd reel_euro2021/bash_scripts/ && sudo chmod +x ./communication.sh && . communication.sh && cd ../../.. && roscore'



echo  "Est ce que vous voulez configurer la communication ros ? print yes ..."
read msg
if [ "$msg" = "yes" ]
then
echo "Donner le hostname de l'autre machine:"
read machine
echo "Donner l'ip de l'autre machine:"
read ad

ip=$(ip a | grep "inet 10.42" | awk -F "inet " '{print $2}' | awk -F "/" '{print $1}')
echo $ip
sudo sed -i '/ROS_MASTER_URI/d' ~/.bashrc
sudo sed -i '/ROS_IP/d' ~/.bashrc
if [ "$1" = "pc" ]
then
echo yess
export ROS_MASTER_URI=http://$ip:11311
sudo echo "export ROS_MASTER_URI=http://$ip:11311" >> ~/.bashrc
elif [ "$1" = "robot" ]
then 
export ROS_MASTER_URI=http://$ad:11311
sudo echo "export ROS_MASTER_URI=http://$ad:11311" >> ~/.bashrc
else
echo "ERRORRR NOT SPECIFYING DEVICE"
exit
fi
export ROS_IP=$ip
sudo echo "export ROS_IP=$ip" >> ~/.bashrc

echo ROS_MASTER_URI: $ROS_MASTER_URI
echo ROS_IP: $ROS_IP

sudo chmod 777 /etc/hosts
cp /etc/hosts /tmp/hosts
y=$(echo -e "127.0.1.1\t$(hostname)")
y2=$(echo -e "$ad\t$machine")
sed -i "/$(hostname)/d" /tmp/hosts
sed -i "/$machine/d" /tmp/hosts
echo $y >> /tmp/hosts
echo $y2 >> /tmp/hosts
sudo mv /tmp/hosts /etc/hosts
echo pre
fi
echo ok




