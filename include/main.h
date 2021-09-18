//
// Created by mwoessner on 18.09.21.
//

#ifndef THEFROG_MAIN_H
#define THEFROG_MAIN_H

void setupServos();
void handleSubmit();
void applyToServo(String key, Servo &servo);
int getBoundedValue(int rawValue);
void connectWifi();
void scanNetworks();
String dereferenceWiFiState(int state);
void beginServer();
String getPage();

#endif //THEFROG_MAIN_H
