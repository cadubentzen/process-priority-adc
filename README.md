Process Priority ADC
========================
This project was developed as part of a university (UFRN) course and demonstrates the change in priority of a process. It uses a A Bleaglebone Black (BBB) that receives analog input through its ADC to control the priority of two processes and visually identify as they change execution time.

## Getting started ##
### Connecting to the Bleaglebone Black ###
On a Linux system
1. Connect your BeagleBone to your computer by using a Mini USB cable.
2. Open the terminal and type `ifconfig`. You should see a network adapter that has one of the following ips `192.168.7.1` or `192.168.6.1`
3. Then the BeagleBone itself will have the ip `192.168.6.2` or `192.168.7.2`
4. Then connect via ssh with the command `ssh debian@192.168.7.2`, use sudo and/or change the ip if necessary. Make sure you're connecting with the correct username in ssh

#### Share internet connection over the USB ####
Reference: <https://elementztechblog.wordpress.com/2014/12/22/sharing-internet-using-network-over-usb-in-beaglebone-black/>

On the BeagleBone Board, run (attempt to the correct ip address, it could be either `192.168.6.#` or `192.168.7.#`)
```
ifconfig usb0 192.168.6.2
route add default gw 192.168.6.1
```

On the PC host, run (ATTENTION: change the `wlan0` to yout internet interface and `eth0` to the network adapter of the BeagleBone. And mind the ip address as well)
```
sudo su
ifconfig eth5 192.168.6.1
iptables --table nat --append POSTROUTING --out-interface wlan0 -j MASQUERADE
iptables --append FORWARD --in-interface eth0 -j ACCEPT
echo 1 > /proc/sys/net/ipv4/ip_forward
```
