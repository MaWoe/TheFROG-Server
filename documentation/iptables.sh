#!/bin/bash

# To be executed on Android device which serves as bridge between mobile network and Arduino
# Purpose: Forward UDP packets incoming on $FROM_INTERFACE / $ORIGINAL_PORT to $ARDUINO_IP / $ARDUINO_PORT
#
# Example (as root):
# ARDUINO_IP=THE_IP bash iptables.sh (bash command may be needed to overcome execute permission restriction)

# From https://serverfault.com/questions/140622/how-can-i-port-forward-with-iptables
# iptables -t nat -A PREROUTING -p tcp -i ppp0 --dport 8001 -j DNAT --to-destination 192.168.1.200:8080
# iptables        -A FORWARD    -p tcp -d 192.168.1.200 --dport 8080 -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT

FROM_INTERFACE="tun0"
ORIGINAL_PORT=3001

ARDUINO_PORT=3000
if [ -z "$ARDUINO_IP" ]; then
  echo "Set ARDUINO_IP env variable first!"
  exit 1
fi

echo
echo "ARDUINO_IP: $ARDUINO_IP"
echo

echo "Table: NAT / chain: PREOUTING"
echo "============================="
OLD_RULE=$(iptables --table nat -S | grep -E "PREROUTING .* -i tun0 -p udp .*")
if [ ! -z "$OLD_RULE" ]; then
  echo "> Deleting old rule first: $OLD_RULE"
  iptables --table nat -D $(echo "$OLD_RULE" | sed -r 's/^-A//')
fi
echo "> Installing new rule"
iptables --table nat  -I PREROUTING 1 -p udp -d 10.77.76.0/24 -i $FROM_INTERFACE --dport $ORIGINAL_PORT -j DNAT --to-destination $ARDUINO_IP:$ARDUINO_PORT
echo

echo "Table: FILTER / chain: FORWARD"
echo "=============================="
OLD_RULE=$(iptables --table filter -S | grep FORWARD | grep '\-d 192.168.43')
if [ ! -z "$OLD_RULE" ]; then
  echo "> Deleting old rule first: $OLD_RULE"
  iptables --table filter -D $(echo "$OLD_RULE" | sed -r 's/^-A//')
fi
echo "> Installing new rule"
iptables --table filter -I FORWARD 1 -p udp -d "$ARDUINO_IP" --dport $ARDUINO_PORT -m state --state INVALID,NEW,RELATED,ESTABLISHED,UNTRACKED -j ACCEPT

exit;

##########

# Logging

iptables --table nat --insert PREROUTING 1 -d 10.77.76.0/24 -i tun0 -p udp -m udp --dport 8001 -j --log-prefix 'nat:PRE: '

FROM_INTERFACE="wlp2s0"
ORIGINAL_PORT=3000

ARDUINO_PORT=3001
ARDUINO_IP="192.168.0.3"

iptables --table nat -A PREROUTING -p udp -i $FROM_INTERFACE --dport $ORIGINAL_PORT -j DNAT --to-destination $ARDUINO_IP:$ARDUINO_PORT
iptables -A FORWARD -p udp -d $ARDUINO_IP --dport $ARDUINO_PORT -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT
