#ifndef THEFROG_MAIN_H
#define THEFROG_MAIN_H

void setupServos();
int getBoundedValue(int rawValue);
void connectWifi();
void scanNetworks();
String dereferenceWiFiState(int state);

#endif //THEFROG_MAIN_H
