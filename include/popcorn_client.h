#ifndef BUTTERRC2_POPCORN_CLIENT_H
#define BUTTERRC2_POPCORN_CLIENT_H

namespace popcorn_api {
    const auto API_PING = "ping";
    const auto API_FULLSCREEN = "togglefullscreen";
    const auto API_PLAY = "toggleplaying";
    const auto API_UP = "up";
    const auto API_DOWN = "down";
    const auto API_LEFT = "left";
    const auto API_RIGHT = "right";
    const auto API_FAVOURITES = "showfavourites";
    const auto API_ENTER = "enter";
    const auto API_BACK = "back";
    const auto API_NEXT_SEASON = "nextseason";
    const auto API_PREVIOUS_SEASON = "previousseason";
    const auto API_QUALITY = "togglequality";

    struct Result {
        int status;
        String content;
    };

    Result call(const char* apiMethod);
}



#endif //BUTTERRC2_POPCORN_CLIENT_H
