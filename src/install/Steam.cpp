//
// Created by werwolf2303 on 6/28/24.
//

#include "Steam.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <QMessageBox>
#include <unistd.h>

#include "../../deps/ValveFileVDF/include/vdf_parser.hpp"
#include "../events/Events.h"
#include "../utils/FileUtils.h"

std::string Steam::getSteamInstallPath() {
    //Linux default steam path = USER_HOME/.local/share/Steam
    std::string steamPath = getpwuid(getuid())->pw_dir;
    steamPath.append("/.local/share/Steam");

    if(!FileUtils::pathExists(std::filesystem::path(steamPath))) {
        return "";
    }

    return steamPath;
}

std::string Steam::getETS2InstallPath(std::string steamPath) {
    std::ifstream vdfFile;
    std::string libraryFoldersPath = steamPath;
    libraryFoldersPath.append("/steamapps/libraryfolders.vdf");
    vdfFile.open(libraryFoldersPath);
    auto parsedVDF = tyti::vdf::read(vdfFile);
    bool foundETS2 = false;
    std::string ETS2Path;

    for(auto& child : parsedVDF.childs) {
        std::string steamLibraryPath = parsedVDF.childs[child.first].get()->attribs["path"];
        std::string ets2Path = "/steamapps/common/Euro Truck Simulator 2";

        foundETS2 = FileUtils::pathExists(steamLibraryPath.append(ets2Path));
        ETS2Path = steamLibraryPath;
    }

    if(!foundETS2) {
        return "GAME";
    }

    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/227300/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        return "PREFIX";
    }

    return ETS2Path;
}


std::string Steam::getATSInstallPath(std::string steamPath) {
    std::ifstream vdfFile;
    std::string libraryFoldersPath = steamPath;
    libraryFoldersPath.append("/steamapps/libraryfolders.vdf");
    vdfFile.open(libraryFoldersPath);
    auto parsedVDF = tyti::vdf::read(vdfFile);
    bool foundATS = false;
    std::string ATSPath;

    for(auto& child : parsedVDF.childs) {
        std::string steamLibraryPath = parsedVDF.childs[child.first].get()->attribs["path"];
        std::string atsPath = "/steamapps/common/American Truck Simulator";

        foundATS = FileUtils::pathExists(steamLibraryPath.append(atsPath));
        ATSPath = steamLibraryPath;
    }

    if(!foundATS) {
        return "GAME";
    }

    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/270880/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        return "PREFIX";
    }

    return ATSPath;
}

std::string Steam::getETS2PrefixPath(std::string steamPath) {
    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/227300/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        return "";
    }

    return prefixPath;
}

std::string Steam::getATSPrefixPath(std::string steamPath) {
    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/270880/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        return "";
    }

    return prefixPath;
}

std::vector<Steam::GameEntry> Steam::getGameList(std::string steamPath) {
    std::vector<Steam::GameEntry> gameList;

    if(steamPath == "") {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "No Steam installation found"));
        return gameList;
    }

    std::string atsGamePath;
    atsGamePath.append(steamPath).append("/steamapps/common/American Truck Simulator");

    if(FileUtils::pathExists(atsGamePath)) {
        GameEntry entry;
        if(getATSPrefixPath(steamPath) == "") {
            entry.name = "American Truck Simulator (No Prefix)";
            entry.gameType = ATS;
            entry.selectable = false;
        }
        if(getATSPrefixPath(steamPath) != "") {
            entry.name = "American Truck Simulator)";
            entry.gameType = ATS;
            entry.selectable = true;
        }
        gameList.push_back(entry);
    }

    std::string ets2GamePath;
    ets2GamePath.append(steamPath).append("/steamapps/common/Euro Truck Simulator 2");

    if(FileUtils::pathExists(ets2GamePath)) {
        GameEntry entry;
        if(getETS2PrefixPath(steamPath) == "") {
            entry.name = "Euro Truck Simulator 2 (No Prefix)";
            entry.gameType = ETS2;
            entry.selectable = false;
        }
        if(getETS2PrefixPath(steamPath) != "") {
            entry.name = "Euro Truck Simulator 2";
            entry.gameType = ETS2;
            entry.selectable = true;
        }
        gameList.push_back(entry);
    }

    return gameList;
}