# TheFROG server

Waits for UDP packets to control my FROG RC car.

## Preparing the receiving Android device to which the Arduino d1 WIFI connects

1. Start OpenVPN
2. Start "VPN Hotspot" app
   * Select "Wi-Fi hotspot"
   * Wait until "Espressif Inc." is in the list of connected clients
3. Determine IP of Arduino connected to your Android's WIFI hotspot
4. Copy `documentation/iptables.sh` to your Android device
5. Run as `root` user `ARDUINO_IP=<the ip> bash documentation/iptables.sh`
   &rarr; This forwards UDP:3001 packets to ARDUINO_IP:3000
6. Start IP cam on Smartphone

## On VPN server machine
1. Copy `documentation/setupNat.sh` to machine
2. Run `sudo ./setupNat.sh`
   &rarr; This forwards UDP:3001 traffic to 10.77.76.20 (receiving device)
3. Run `sudo PROTO=tcp IN_PORT=7776 TO_PORT=8080 ./setupNat.sh`
   &rarr; This allows for accessing IPCam on 10.77.76.20:8080 via vpn-server:7776


## Hardware

d1 WIFI

Pin connectors:
* D2: Steering
* D3: Drive

## Repositories involved
* This one (https://github.com/MaWoe/TheFROG-Server)
* https://github.com/MaWoe/TheFROG-App