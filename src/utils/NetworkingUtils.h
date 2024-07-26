//
// Created by werwolf2303 on 6/29/24.
//

#ifndef NETWORKINGUTILS_H
#define NETWORKINGUTILS_H
#include <functional>
#include <QByteArray>

#include "ErrorHandling.h"


class NetworkingUtils {
public:
    enum Endpoints  {
        API,
        DOWNLOAD,
        UPDATE,
        CUSTOM,
        GITHUB
    };

    static QByteArray makeGet(Endpoints server, std::string path);
    static std::string getLocation(std::string path);
    static bool download(Endpoints server, const std::string& path, std::string downloadTo);
};



#endif //NETWORKINGUTILS_H
