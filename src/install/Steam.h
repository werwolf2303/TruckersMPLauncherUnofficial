//
// Created by werwolf2303 on 6/28/24.
//

#ifndef STEAM_H
#define STEAM_H
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "../utils/ErrorHandling.h"


class Steam {
public:
    enum GameEntryGameType {
        ETS2,
        ATS
    };
    class GameEntry {
    public:
        std::string name;
        bool selectable;
        GameEntryGameType gameType;
    };
    static std::string getSteamInstallPath();
    static std::string getETS2InstallPath(std::string steamPath);
    static std::string getATSInstallPath(std::string steamPath);
    static std::string getETS2PrefixPath(std::string steamPath);
    static std::string getATSPrefixPath(std::string steamPath);
    static std::vector<GameEntry> getGameList(std::string steamPath);
};



#endif //STEAM_H
