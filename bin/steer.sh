#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 KEY DEGREE";
  exit 1
fi

KEY=$1
ANGLE=$2

echo "Setting $KEY to angle $ANGLE ..."
curl http://arduino.fritz.box/?$KEY=$ANGLE
