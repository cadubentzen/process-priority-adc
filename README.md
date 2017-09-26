Process Priority ADC
========================
This project was developed as part of a university (UFRN) course and demonstrates the change in priority of a process. It uses a A Bleaglebone Black (BBB) that receives analog input through its ADC to control the priority of two processes and visually identify as they change execution time.

![BeagleBone](http://beagleboard.org/static/images/cape-headers.png)

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
ifconfig eth0 192.168.6.1
iptables --table nat --append POSTROUTING --out-interface wlan0 -j MASQUERADE
iptables --append FORWARD --in-interface eth0 -j ACCEPT
echo 1 > /proc/sys/net/ipv4/ip_forward
```
## Enable ADC read in Kernel 4.x ##

With root provileges, run
```
sh -c "echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots"
```

And to read from analog port, run
```
cat "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
```
in `in_voltage0_raw` change 0 to the number of the ADC, from 0 to 6.

See references to make this permanent on reboot

### Description in Portuguese ###
Descrição do programa:

- Gere 03 processos: Pai, Filho1, Filho2

- Processo Pai:
     - Este processo terá prioridade 0.
     - Lerá a cada 500ms 02 portas analógicas (AD1, AD2) da Beagle.
     - Se o valor de tensão da porta AD1 for maior que 1V, então setar a prioridade do processo Filho1 para 05. Caso contrário, setar a prioridade do processo Filho1 para 19.
     - Se o valor de tensão da porta AD2 for maior que 1V, então setar a prioridade do processo Filho2 para 05. Caso contrário, setar a prioridade do processo Filho2 para 19.


- Processos Filhos (Filho1 e Filho2) executarão num loop infinito uma função carga (configurar esta função para que ela execute em torno de 1s).

- Nas iterações pares no seu loop infinito, o processo Filho (Filho 1 ou Filho2) deverá acender um Led (Led1, para Filho1 e Led2, para Filho2).

- Nas iterações ímpares no seu loop infinito, o processo Filho (Filho 1 ou Filho2) deverá apagar um Led (Led1, para Filho1 e Led2, para Filho2).

Material necessário:
- 02 leds
- 02 potenciômetros
- 02 resistores
- kit beagle board

ATENÇÃO: Para não queimar a placa, utilizar o pinos exclusivos do ADC (pino VDD_ADC -1.8V, pino GND_ADC - terra).

Gerar vídeo demonstrando os resultados do trabalho. Alterar valores de prioridade dos processos e verificar os seus efeitos.

## References ##
1. ADC on BeagleBone. <http://beaglebone.cameon.net/home/reading-the-analog-inputs-adc>
2. How to read ADC values in kernel 4.x. <https://askubuntu.com/questions/882433/enable-adc-input-on-beaglebone-with-4-x-kernel>
