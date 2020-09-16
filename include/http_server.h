#ifndef BUTTERRC2_HTTP_SERVER_H
#define BUTTERRC2_HTTP_SERVER_H

namespace http_server {
    void init();
    void handleClients(); // should be called each global loop iteration
}

#endif //BUTTERRC2_HTTP_SERVER_H
