//
// Created by werwolf2303 on 6/28/24.
//

#ifndef LAUNCHER_H
#define LAUNCHER_H
#include <functional>
#include <string>

#include "../api/TruckersMPAPI.h"
#include "../utils/ErrorHandling.h"

class Launcher {
public:
    static bool downloadTMPFiles(std::string prefixPath,
        TruckersMPAPI::GameTypes gameType);
    static bool verifyTMPFiles(std::string prefixPath,
        TruckersMPAPI::GameTypes gameType);
    static bool checkFoldersAndFiles(std::string prefixPath);
    static bool injectTruckersMP(std::string prefixPath, std::string gamePath, char *argv[]);
};



#endif //LAUNCHER_H
