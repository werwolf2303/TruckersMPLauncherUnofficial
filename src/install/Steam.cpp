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
#include "../utils/FileUtils.h"

std::string Steam::getSteamInstallPath() {
    //Linux default steam path = USER_HOME/.local/share/Steam
    std::string steamPath = getpwuid(getuid())->pw_dir;
    steamPath.append("/.local/share/Steam");

    if(!FileUtils::pathExists(std::filesystem::path(steamPath))) {
        QMessageBox::critical(nullptr, "Critical", "Couldn't find Steam");
        exit(-1);
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
        QMessageBox::critical(nullptr, "Critical", "Couldn't find ETS2");
        exit(-1);
    }

    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/227300/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        QMessageBox::critical(nullptr, "Critical", "Couldn't find proton prefix for ETS2");
        exit(-1);
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
        QMessageBox::critical(nullptr, "Critical", "Couldn't find ATS");
        exit(-1);
    }

    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/270880/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        QMessageBox::critical(nullptr, "Critical", "Couldn't find proton prefix for ATS");
        exit(-1);
    }

    return ATSPath;
}

std::string Steam::getETS2PrefixPath(std::string steamPath) {
    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/270880/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        QMessageBox::critical(nullptr, "Critical", "Couldn't find proton prefix for ATS");
        exit(-1);
    }

    return prefixPath;
}

std::string Steam::getATSPrefixPath(std::string steamPath) {
    std::string prefixPath;
    prefixPath.append(steamPath).append("/steamapps/compatdata/227300/pfx/drive_c");

    if(!FileUtils::pathExists(prefixPath)) {
        QMessageBox::critical(nullptr, "Critical", "Couldn't find proton prefix for ETS2");
        exit(-1);
    }

    return prefixPath;
}