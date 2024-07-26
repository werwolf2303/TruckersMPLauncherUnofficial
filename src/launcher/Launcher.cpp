//
// Created by werwolf2303 on 6/28/24.
//

#include "Launcher.h"

#include <iostream>
#include <QMessageBox>
#include <sys/stat.h>

#include "elzip.hpp"
#include "../api/TruckersMPAPI.h"
#include "../events/Events.h"
#include "../install/Steam.h"
#include "../utils/FileUtils.h"
#include "../utils/NetworkingUtils.h"

bool Launcher::checkFoldersAndFiles(std::string prefixPath) {
    if(prefixPath == "") {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Game prefix not found"));
        return false;
    }
    std::string truckersMPPath;
    truckersMPPath.append(prefixPath);
    truckersMPPath.append("/users/steamuser/AppData/Local/TruckersMP");

    if (!FileUtils::pathExists(truckersMPPath)) {
        try {
            if (!std::filesystem::create_directory(std::filesystem::path(truckersMPPath))) {
                Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Couldn't create TruckersMP folder"));
                return false;
            }
        }catch(std::exception& exception) {
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, exception.what()));
            return false;
        }
    }

    std::string protonPath;
    protonPath.append(truckersMPPath);
    protonPath.append("/GE-Proton9-6");

    //GE-Proton9-6.zip
    //URL: https://github.com/werwolf2303/TruckersMPLauncherUnofficial/releases/latest/download/GE-Proton9-6.zip
    if (!FileUtils::pathExists(protonPath)) {
        std::string protonZipPath;
        protonZipPath.append(protonPath).append(".zip");
        if (!FileUtils::pathExists(protonZipPath))
            if (!NetworkingUtils::download(
                NetworkingUtils::GITHUB,
                "https://github.com/werwolf2303/TruckersMPLauncherUnofficial/releases/download/files/GE-Proton9-6.zip",
                protonZipPath
            )) {
                Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Failed to download proton"));
                return false;
            }
        try {
            elz::extractZip(elz::path(protonZipPath), elz::path(truckersMPPath));
        } catch (...) {
            std::filesystem::remove(protonZipPath);
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Failed extracting proton"));
            return false;
        }
    }


    std::string tmpCLI;
    tmpCLI.append(truckersMPPath);
    tmpCLI.append("/truckersmp-cli.exe");

    //truckersmp-cli.exe
    //URL: https://github.com/werwolf2303/TruckersMPLauncherUnofficial/releases/download/files/truckersmp-cli.exe
    if (!FileUtils::pathExists(tmpCLI)) {
        if (!NetworkingUtils::download(
                NetworkingUtils::GITHUB,
                "https://github.com/werwolf2303/TruckersMPLauncherUnofficial/releases/download/files/truckersmp-cli.exe",
                tmpCLI
        )) {
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::ErrorType::GENERIC, "Failed to download TruckersMPCLI"));
            return false;
        }
    }
    return true;
}

bool Launcher::downloadTMPFiles(std::string prefixPath, TruckersMPAPI::GameTypes gameType) {
    for(TruckersMPAPI::Files::File file : TruckersMPAPI::Files::getFiles(gameType)) {
        std::string downloadToPath;
        downloadToPath.append(prefixPath);
        downloadToPath.append("/users/steamuser/AppData/Local/TruckersMP");
        downloadToPath.append(file.filePath);
        std::filesystem::path filePath(downloadToPath);
        if(!FileUtils::pathExists(filePath.parent_path())) std::filesystem::create_directories(filePath.parent_path());
        NetworkingUtils::download(NetworkingUtils::DOWNLOAD, QString("/files").append(file.filePath.c_str()).toStdString(), filePath);
    }
}

bool Launcher::verifyTMPFiles(std::string prefixPath, TruckersMPAPI::GameTypes gameType) {
    for(TruckersMPAPI::Files::File file : TruckersMPAPI::Files::getFiles(gameType)) {
        std::string downloadToPath;
        downloadToPath.append(prefixPath);
        downloadToPath.append("/users/steamuser/AppData/Local/TruckersMP");
        downloadToPath.append(file.filePath);
        std::filesystem::path filePath(downloadToPath);
        if(file.md5 != FileUtils::calculateMD5(downloadToPath)) {
            if(!FileUtils::pathExists(filePath.parent_path())) std::filesystem::create_directories(filePath.parent_path());
            NetworkingUtils::download(NetworkingUtils::DOWNLOAD, QString("/files").append(file.filePath.c_str()).toStdString(), filePath);
        }
    }
}

std::vector<std::string> split(std::string_view str, char delim)
{
    std::vector<std::string> result;
    auto left = str.begin();
    for (auto it = left; it != str.end(); ++it)
    {
        if (*it == delim)
        {
            result.emplace_back(&*left, it - left);
            left = it + 1;
        }
    }
    if (left != str.end())
        result.emplace_back(&*left, str.end() - left);
    return result;
}

bool Launcher::injectTruckersMP(std::string prefixPath, std::string gamePath, char *argv[]) {
    if(gamePath == "") {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Game path not found"));
        return false;
    }
    std::string truckersMPPath;
    truckersMPPath.append(prefixPath);
    truckersMPPath.append("/users/steamuser/AppData/Local/TruckersMP");

    setenv("STEAM_COMPAT_DATA_PATH", std::filesystem::path(prefixPath.c_str()).parent_path().parent_path().c_str(), 1);
    setenv("STEAM_COMPAT_CLIENT_INSTALL_PATH", Steam::getSteamInstallPath().c_str(), 1);
    for(int i = 0; i < sizeof(argv); i++) {
        if(argv[i] == nullptr) break;
        std::string argString(argv[i]);
        if(argString.find("=") == std::string::npos) continue;
        std::string leftHand = split(argString, '=')[0];
        std::string rightHand = split(argString, '=')[1];
        setenv(leftHand.c_str(), rightHand.c_str(), 1);
    }

    std::string constructedProtonPath;
    constructedProtonPath.append(prefixPath).append("/users/steamuser/AppData/Local/TruckersMP/GE-Proton9-6/proton");

    int protonExecRightsSetResult = chmod(constructedProtonPath.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
    if(protonExecRightsSetResult != 0) {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Failed to set executable bit for proton\n"));
        return false;
    }

    for (std::filesystem::recursive_directory_iterator i(truckersMPPath), end; i != end; ++i) {
        if (!is_directory(i->path())) {
            int fileExecRightSetResult = chmod(i->path().c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
            if(fileExecRightSetResult != 0) {
                Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Failed to set executable bit for proton\n"));
                return false;
            }
        }
    }

    std::string constructedTMPCLIPath;
    constructedTMPCLIPath.append(prefixPath).append("/users/steamuser/AppData/Local/TruckersMP/truckersmp-cli.exe");

    std::string constructedCommand;
    constructedCommand.append("\"");
    constructedCommand.append(constructedProtonPath);
    constructedCommand.append("\" run \"");
    constructedCommand.append(constructedTMPCLIPath);
    constructedCommand.append("\" \"");
    constructedCommand.append(gamePath);
    constructedCommand.append("\" \"");
    constructedCommand.append(truckersMPPath);
    constructedCommand.append("\"");

    std::system(constructedCommand.c_str());
    return true;
}



