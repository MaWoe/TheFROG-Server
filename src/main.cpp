#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Streaming-6.0.9/src/Streaming.h"
#include "helpers.h"
#include "main.h"
#include "secrets.h"
#include "wemos_pins.h"

const int Servo_Drive_pin = WEMOS_D3;
const int Servo_Steering_pin = WEMOS_D2;

const unsigned int LOCAL_UDP_PORT = 3000;
const int UDP_SCAN_DELAY_MS = 2;

/**
 * Format:
 *
 * Steering ----+
 * Drive ---+   |
 * Sequence |   |
 * \_______ \__ \__
 * 000001AE:090:080
 */
const int UDP_IN_BUFFER_SIZE = 16;
char udpInBuffer[UDP_IN_BUFFER_SIZE + 1];

WiFiUDP udp;

Servo Servo_Drive;
Servo Servo_Steering;

int timeOfLastReception = 0;
bool fullyStoppedAfterReceptionTimeout = false;

void setup() {
    Serial.begin(9600);
    delay(200);

    setupServos();
    scanNetworks();
    connectWifi();

    udp.begin(LOCAL_UDP_PORT);
}

void loop() {
    if (!readValidPacketIntoBuffer()) {
        slowDownOnPacketReceptionTimeout();
        delay(UDP_SCAN_DELAY_MS);
        return;
    }

    timeOfLastReception = millis();
    fullyStoppedAfterReceptionTimeout = false;

    String udpBody = String(udpInBuffer);

    int packetSequence = udpBody.substring(0, 9).toInt();
    int drive = getBoundedValue(udpBody.substring(9, 12).toInt());
    int steering = getBoundedValue(udpBody.substring(13, 16).toInt());

    Servo_Drive.write(drive);
    Servo_Steering.write(steering);
}

bool readValidPacketIntoBuffer() {
    int packetSize = udp.parsePacket();
    if (packetSize == 0) {
        return false;
    } else if (packetSize != UDP_IN_BUFFER_SIZE) {
        Serial << "[" << millis() << "] Mismatch: Packet size " << packetSize << " <> " << UDP_IN_BUFFER_SIZE << endl;
        return false;
    }

    int bytesRead = udp.read(udpInBuffer, UDP_IN_BUFFER_SIZE);
    if (bytesRead != UDP_IN_BUFFER_SIZE) {
        Serial << "[" << millis() << "] Mismatch: Bytes read " << bytesRead << " <> " << UDP_IN_BUFFER_SIZE << endl;
        return false;
    }

    udpInBuffer[bytesRead] = 0;
    return true;
}

void slowDownOnPacketReceptionTimeout() {
    int timeSinceLastReception = millis() - timeOfLastReception;
    if (!fullyStoppedAfterReceptionTimeout && timeSinceLastReception > 3000) {
        // Improve me (slow down in steps):
        Servo_Drive.write(90);
        fullyStoppedAfterReceptionTimeout = true;
    }
}

void setupServos() {
    Serial.println("Setting up servos");

    Servo_Drive.attach(Servo_Drive_pin, 1000, 2000);
    Servo_Drive.write(90);

    Servo_Steering.attach(Servo_Steering_pin, 1000, 2000);
    Servo_Steering.write(90);
}

int getBoundedValue(int rawValue) {
    if (rawValue > 180) {
        // Zu groß!
        return 180;
    } else if (rawValue < 0) {
        // Zu klein!
        return 0;
    } else {
        return rawValue;
    }
}

void connectWifi() {
    Serial.println("Attempting WIFI connection");
    WiFi.begin(ssid, password);
    int lastStatus = 999;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        int currentStatus = WiFi.status();
        if (lastStatus != currentStatus) {
            lastStatus = currentStatus;
            Serial.println(dereferenceWiFiState(currentStatus));
        }
    }
    Serial.println("");
    Serial.println("WiFi verbunden");
    Serial.println("IP Addresse: ");
    Serial.println(WiFi.localIP());
}

void scanNetworks() {
    // scan for nearby networks:
    Serial.println("** Scan Networks **");
    byte numSsid = WiFi.scanNetworks();

    Serial << "SSID List:" << numSsid << endl;

    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        Serial << thisNet << ") Network: " << WiFi.SSID(thisNet) << endl;
    }
}
