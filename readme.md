# PopRC
PopRC is a remote control designed for controlling the PopcornTime app
The code is the firmware to be flashed on a Esp8266 NodeMCU board
Beside PopcornTime, PopRC can also control VLC (if lua http interface is enabled in vlc)

When PopRC boots, it tries to connect to it's configured WIFI network.
In a case of failure it will start it's own WiFi Access Point named "PopRC" (can be configured).
to communicate and reconfigure PopRC, you should connect to it's WIFI network and update it's configuration (which is explained later)

PopRC will have the mDns hostname "poprc" (which is also configurable)
so to connect to it's http api / interface without finding out it's IP
you can use hostname
- `poprc` - on windows systems
- `poprc.local` - on linux systems


## Serial Commands
- `reset` / `restart` - will restart the controller
- `heap` - will print the free heap space left
- `config` - will print "safe" string of the configuration (without sensitive values)
- `set {KEY=VALUE}` - will set a new value to a configuration field

## HTTP API
- `/config` - return a "safe" string of the configuration (without sensitive values)
- `/updateConfig?key1=value1&key2=value2` - update one or more config values
- `/reset` OR `/restart` - restart the controller


## Troubleshooting
### PopRC is connected to my WIFI network and has correct PC_IP set but pressing the keys doesnt do anything
Check your firewall settings, in windows make sure that popcorntime.exe is allowed to have incoming connections

### PopRC is connected to my WIFI network and has correct PC_IP set but some/all keys work first time and then don't work for approx 1/2 min
This can happen on windows because of the firewall's "stealth mode" which doesn't to new connections to a closed port (instead of the normal "Refusing Connection" behavior)
Fixing this is a matter of updating the registry and a reboot
Follow [this page](https://support.microsoft.com/en-au/help/2586744/disable-stealth-mode) for a thorough instructions
