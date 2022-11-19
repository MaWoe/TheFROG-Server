#ifndef THEFROG_HELPERS_H
#define THEFROG_HELPERS_H

String dereferenceWiFiState(int state) {
    switch (state) {
        case WL_CONNECTED:
            return "L_CONNECTED";
        case WL_NO_SHIELD:
            return "L_NO_SHIELD";
        case WL_IDLE_STATUS:
            return "L_IDLE_STATUS";
        case WL_NO_SSID_AVAIL:
            return "L_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED:
            return "L_SCAN_COMPLETED";
        case WL_CONNECT_FAILED:
            return "L_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
            return "L_CONNECTION_LOST";
        case WL_DISCONNECTED:
            return "L_DISCONNECTED";
        default:
            return "Unknown state";
    }
}

bool tryConnect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
    int lastStatus = 999;
    int tries = 1;
    Serial.printf("Try connecting to SSID \"%s\" ...\n", ssid);
    while (WiFi.status() != WL_CONNECTED && tries <= 5) {
        delay(500);
        int currentStatus = WiFi.status();
        if (lastStatus != currentStatus) {
            lastStatus = currentStatus;
            Serial << "> #" << tries << ": " << dereferenceWiFiState(currentStatus) << endl;
            if (currentStatus == WL_NO_SSID_AVAIL) {
                Serial.println("> Giving up!");
                return false;
            }
            tries++;
        }
    }

    bool success = WiFi.status() == WL_CONNECTED;

    if (success) {
        Serial.println("OK - IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("FAIL\n");
    }

    return success;
}

#endif //THEFROG_HELPERS_H
