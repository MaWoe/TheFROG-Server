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

#endif //THEFROG_HELPERS_H
