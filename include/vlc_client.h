#ifndef BUTTERRC2_VLC_CLIENT_H
#define BUTTERRC2_VLC_CLIENT_H

namespace vlc_api {
    const auto API_PLAY = "status.xml?command=pl_pause";
    const auto API_FULLSCREEN = "status.xml?command=fullscreen";
    const auto API_SEEK_FORWARD="status.xml?command=seek&val=+10S";
    const auto API_SEEK_BACKWARD="status.xml?command=seek&val=-10S";
    const auto API_VOL_UP="status.xml?command=volume&val=+10";
    const auto API_VOL_DOWN="status.xml?command=volume&val=-10";

    int call(const char* apiMethod);
}

#endif //BUTTERRC2_VLC_CLIENT_H
