//
// Created by werwolf2303 on 6/28/24.
//

#ifndef STEAM_H
#define STEAM_H
#include <filesystem>
#include <string>


class Steam {
public:
    static std::string getSteamInstallPath();
    static std::string getETS2InstallPath(std::string steamPath);
    static std::string getATSInstallPath(std::string steamPath);
    static std::string getETS2PrefixPath(std::string steamPath);
    static std::string getATSPrefixPath(std::string steamPath);
};



#endif //STEAM_H
