# TheFROG server

Waits for UDP packets to control my FROG RC car.

## Preparations on the Android Smartphone that the Arduino d1 WIFI connects to

1. Start OpenVPN on your Android

2. Start "VPN Hotspot" app

   * Select "Wi-Fi hotspot"

3. Determine IP of Arduino connected to your Android's WIFI hotspot

4. Run `documentation/iptables.sh` on your Android via ADB shell as `root` user:

   ` ARDUINO_IP=<the ip> documentation/iptables.sh`

5. Start IP cam on Smartphone

   