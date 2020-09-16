#include "../include/config.h"
#include <FS.h>


// definition for namespace values 'extern'-declared in the header file
namespace config {
    String WIFI_SSID;
    String WIFI_PASS;
    String PC_IP;
    String LOCAL_HOSTNAME;
    String POPCORN_PORT;
    String POPCORN_AUTH_USER;
    String POPCORN_AUTH_PASS;
    String VLC_PORT;
    String AP_SSID;
    String AP_PASS;
    IPAddress AP_LOCAL_IP;
    IPAddress AP_GATEWAY;
    IPAddress AP_NETMASK;
    bool DEBUG;
}


using namespace config::parsing;
#define IF_MATCH_SET_AS_STRING(KEY, CONFIG_NAME) if (KEY == #CONFIG_NAME) config::CONFIG_NAME = value;
#define IF_MATCH_SET_AS_IPADDRESS(KEY, CONFIG_NAME) if (KEY == #CONFIG_NAME) config::CONFIG_NAME = parseIp(value);
#define IF_MATCH_SET_AS_BOOL(KEY, CONFIG_NAME) if (KEY == #CONFIG_NAME) config::CONFIG_NAME = parseBool(value);

#define STR_CONFIG_LINE(CONFIG_NAME) (#CONFIG_NAME + String("=") + config::CONFIG_NAME)
#define IP_CONFIG_LINE(CONFIG_NAME) (#CONFIG_NAME + String("=") + config::CONFIG_NAME.toString())


const auto CONFIG_PATH = "/config.conf";

namespace config {
    bool load() {
        SPIFFS.begin();

        bool retValue;
        File confFile = SPIFFS.open(CONFIG_PATH, "r");
        {
            if (!confFile) {
                Serial.println("Failed opening config.conf file!");
                return false; // no need to cleanup
            }

            String fileContent = confFile.readString();
            retValue = parsing::setFromConfFormat(fileContent);

        }
        confFile.close();
        return retValue;
    }

    bool dump() {
        SPIFFS.begin();

        File confFile = SPIFFS.open(CONFIG_PATH, "w");
        {
            if (!confFile) {
                Serial.println("Failed opening config.conf file!");
                return false; // no need to cleanup
            }

            confFile.println(STR_CONFIG_LINE(WIFI_SSID));
            confFile.println(STR_CONFIG_LINE(WIFI_PASS));
            confFile.println(STR_CONFIG_LINE(PC_IP));
            confFile.println(STR_CONFIG_LINE(LOCAL_HOSTNAME));
            confFile.println(STR_CONFIG_LINE(POPCORN_PORT));
            confFile.println(STR_CONFIG_LINE(POPCORN_AUTH_USER));
            confFile.println(STR_CONFIG_LINE(POPCORN_AUTH_PASS));
            confFile.println(STR_CONFIG_LINE(VLC_PORT));
            confFile.println(STR_CONFIG_LINE(AP_SSID));
            confFile.println(STR_CONFIG_LINE(AP_PASS));
            confFile.println(IP_CONFIG_LINE(AP_LOCAL_IP));
            confFile.println(IP_CONFIG_LINE(AP_GATEWAY));
            confFile.println(IP_CONFIG_LINE(AP_NETMASK));
            confFile.println(STR_CONFIG_LINE(DEBUG));
        }
        confFile.close();
        return true;
    }

    String getSafeConfigString() {
        const char endLine = '\n';
        return String() +
            STR_CONFIG_LINE(WIFI_SSID) + endLine +
            // intentionally don't return WIFI_PASS
            STR_CONFIG_LINE(PC_IP) + endLine +
            STR_CONFIG_LINE(LOCAL_HOSTNAME) + endLine +
            STR_CONFIG_LINE(POPCORN_PORT) + endLine +
            STR_CONFIG_LINE(POPCORN_AUTH_USER) + endLine +
            STR_CONFIG_LINE(POPCORN_AUTH_PASS) + endLine +
            STR_CONFIG_LINE(VLC_PORT) + endLine +
            STR_CONFIG_LINE(AP_SSID) + endLine +
            STR_CONFIG_LINE(AP_PASS) + endLine +
            IP_CONFIG_LINE(AP_LOCAL_IP) + endLine +
            IP_CONFIG_LINE(AP_GATEWAY) + endLine +
            IP_CONFIG_LINE(AP_NETMASK) + endLine +
            STR_CONFIG_LINE(DEBUG); // bool works with STR
    }

    namespace parsing {
        IPAddress parseIp(const String &ipStr) {
            String parts[4];
            int lastDotIndex = -1;
            for (int i = 0; i < 4; i++) {
                int nextDotIndex = ipStr.indexOf('.', lastDotIndex + 1);
                parts[i] = ipStr.substring(lastDotIndex + 1, nextDotIndex);
                lastDotIndex = nextDotIndex; // the old 'next' is now the last
            }

            return IPAddress(
                    atoi(parts[0].c_str()),
                    atoi(parts[1].c_str()),
                    atoi(parts[2].c_str()),
                    atoi(parts[3].c_str())
            );
        }

        bool parseBool(const String &ipStr) {
            return (ipStr == "1");
        }

        bool setFromConfFormat(String confString) {
            confString.trim();

            int lineNum = 0;
            unsigned int i = 0;
            while (i < confString.length()) {
                int endOfLineIndex = confString.indexOf('\n', i);

                // last line
                if (endOfLineIndex < 0)
                    // potential problem with this conversion but fuck it
                    endOfLineIndex = (int)confString.length();

                String line = confString.substring(i, endOfLineIndex);
                line.trim();

                if (line != "") {
                    int equalsSignIndex = line.indexOf('=');

                    if (equalsSignIndex < 0) {
                        auto error = "Failed parsing configuration! (missing '=' in line " + String(lineNum) + ")";
                        Serial.println(error);
                        return false;
                    }

                    String key = line.substring(0, equalsSignIndex);
                    String value = line.substring(equalsSignIndex + 1, line.length());

                    IF_MATCH_SET_AS_STRING(key, WIFI_SSID)
                    else IF_MATCH_SET_AS_STRING(key, WIFI_PASS)
                    else IF_MATCH_SET_AS_STRING(key, PC_IP)
                    else IF_MATCH_SET_AS_STRING(key, LOCAL_HOSTNAME)
                    else IF_MATCH_SET_AS_STRING(key, POPCORN_PORT)
                    else IF_MATCH_SET_AS_STRING(key, POPCORN_AUTH_USER)
                    else IF_MATCH_SET_AS_STRING(key, POPCORN_AUTH_PASS)
                    else IF_MATCH_SET_AS_STRING(key, VLC_PORT)
                    else IF_MATCH_SET_AS_STRING(key, AP_SSID)
                    else IF_MATCH_SET_AS_STRING(key, AP_PASS)
                    else IF_MATCH_SET_AS_IPADDRESS(key, AP_LOCAL_IP)
                    else IF_MATCH_SET_AS_IPADDRESS(key, AP_GATEWAY)
                    else IF_MATCH_SET_AS_IPADDRESS(key, AP_NETMASK)
                    else IF_MATCH_SET_AS_BOOL(key, DEBUG)
                    else {
                        Serial.println("Failed to parse configuration! Encountered unfamiliar key: " + key);
                        return false;
                    }
                }

                i = endOfLineIndex + 1;
                lineNum++;
            }

            return true;
        }
    }
}
