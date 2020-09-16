#ifndef BUTTERRC2_CONFIG_H
#define BUTTERRC2_CONFIG_H


#include <IPAddress.h>

namespace config {
    extern String WIFI_SSID;
    extern String WIFI_PASS;
    extern String PC_IP;
    extern String LOCAL_HOSTNAME;

    // popcorn specific
    extern String POPCORN_PORT;
    extern String POPCORN_AUTH_USER;
    extern String POPCORN_AUTH_PASS;

    // vlc specific
    extern String VLC_PORT;

    // fallback AP
    extern String AP_SSID;
    extern String AP_PASS;
    extern IPAddress AP_LOCAL_IP;
    extern IPAddress AP_GATEWAY;
    extern IPAddress AP_NETMASK;

    extern bool DEBUG;

    bool load(); // loads the config from FS
    bool dump(); // dumps config from RAM to FS

    String getSafeConfigString(); // safe as in the string doesnt contain sensitive values

    namespace parsing {
        IPAddress parseIp(const String& ipStr);
        bool parseBool(const String& boolStr);

        bool setFromConfFormat(String confString);
    }
}


#endif //BUTTERRC2_CONFIG_H
