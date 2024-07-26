//
// Created by werwolf2303 on 6/30/24.
//

#ifndef BACKENDSERVER_H
#define BACKENDSERVER_H
#include <string>
#include <thread>
#include <QHttpServer>

class FrontendServer {
public:
    bool start();
    bool stop();
    bool init();
private:
    QHttpServer server;
    static std::vector<std::string> split(std::string str, char delim);
};

#endif //BACKENDSERVER_H
