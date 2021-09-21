#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <drive>:<steering>";
  echo
  echo "drive and steering are both 3 digit values between 000 and 180"
  exit 1
fi

echo -n "$1" > /dev/udp/arduino.fritz.box/3000
