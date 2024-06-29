//
// Created by werwolf2303 on 6/28/24.
//

#include "Launcher.h"

#include <iostream>
#include <QMessageBox>

#include "../utils/FileUtils.h"

void Launcher::checkFoldersAndFiles(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress) {
    int maxProg = 100;
    int minProg = 0;
    int nowProg = 0;

    std::string truckersMPPath;
    truckersMPPath.append(prefixPath);
    truckersMPPath.append("/users/steamuser/AppData/Local/TruckersMP");

    if(!FileUtils::pathExists(truckersMPPath)) {
        if(!std::filesystem::create_directory(std::filesystem::path(truckersMPPath))) {
            QMessageBox::critical(nullptr, "Critical", "Couldn't create TruckersMP folder");
            exit(-1);
        }
    }

    std::string protonPath;
    protonPath.append(truckersMPPath);
    protonPath.append("/GE-Proton-9-6");

    //GE-Proton9-6.zip
    if(!FileUtils::pathExists(protonPath)) {
        //if(!std::filesystem::copy_file(
        //    std::filesystem::path();
        //)) {
        //    QMessageBox::critical(nullptr, "Critical", "Couldn't copy proton");
        //    exit(-1);
        //}
    }


    std::string tmpCLI;
    tmpCLI.append(truckersMPPath);

    //truckersmp-cli.exe
    if(!FileUtils::pathExists(tmpCLI)) {

    }
}
