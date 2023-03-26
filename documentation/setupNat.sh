#!/bin/bash

# To be executed on machine where VPN server is running
# Purpose: Forward UDP packets incoming on $IN_IP / $IN_PORT on first external interface to
# $TO_IP / $TO_PORT

getFirstExternalIp() {
  local ip
  for ip in $(hostname -I); do
    if echo $ip | grep -vEq '^(10|127)\.'; then
      echo $ip
      return
    fi
  done
}

IF=${IF:-venet0}
PROTO=${PROTO:-udp}

IN_IP=${IN_IP:-$(getFirstExternalIp)}
IN_PORT=${IN_PORT:-3001}

TO_IP=${TO_IP:-10.77.76.20}
TO_PORT=${TO_PORT:-3001}

echo
echo "IF:      $IF"
echo "PROTO:   $PROTO"
echo "IN_IP:   $IN_IP"
echo "IN_PORT: $IN_PORT"
echo "TO_IP:   $TO_IP"
echo "TO_PORT: $TO_PORT"
echo
#exit;

iptables --table nat -A PREROUTING -d "${IN_IP}/32" -i $IF -p $PROTO -m $PROTO --dport $IN_PORT -j DNAT --to-destination "$TO_IP:$TO_PORT"
iptables --table filter -A FORWARD -d "${TO_IP}/32"        -p $PROTO -m $PROTO --dport $TO_PORT -m state --state INVALID,NEW,RELATED,ESTABLISHED,UNTRACKED -j ACCEPT

