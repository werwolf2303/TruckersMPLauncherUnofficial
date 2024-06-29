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
        UPDATE
    };

    static QByteArray makeGet(Endpoints server, const std::string& path, std::function<int(ErrorHandling::ErrorType errorType, std::string)> onError);
    static bool download(Endpoints server, const std::string& path, std::string downloadTo, std::function<int(ErrorHandling::ErrorType errorType, std::string)> onError,
        std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription, std::string downloadSpeed)> onProgress = nullptr);
};



#endif //NETWORKINGUTILS_H
