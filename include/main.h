#ifndef THEFROG_MAIN_H
#define THEFROG_MAIN_H

void setupServos();
int getBoundedValue(int rawValue);
void connectWifi();
void scanNetworks();
String dereferenceWiFiState(int state);
bool readValidPacketIntoBuffer();
void slowDownOnPacketReceptionTimeout();

#endif //THEFROG_MAIN_H
