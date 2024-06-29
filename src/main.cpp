#include <iostream>
#include "api/TruckersMPAPI.h"
#include <QApplication>
#include <QMessageBox>
#include "install/Steam.h"
#include "launcher/Launcher.h"
#include "utils/FileUtils.h"
#include "utils/NetworkingUtils.h"
#include "utils/OSDetect.h"

int onError(ErrorHandling::ErrorType errorType, std::string message) {
    std::cout << "Error happened in TruckersMP api (Descriptive? " << (errorType == ErrorHandling::DESCRIPTIVE || errorType == ErrorHandling::DESCRIPTIVE_API) << ") : " + message << "\n";

    return 0;
}

int onProgressUpdate(int maxProgress, int minProgress, int actualProgress, std::string progressDescription, std::string downloadSpeed) {
    std::cout << progressDescription;
    std::cout << " Min:" << minProgress;
    std::cout << " Current:" << actualProgress;
    std::cout << " Max:" << maxProgress;
    std::cout << " Download speed:" << downloadSpeed;
    std::cout << "\n";

    return 0;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if(OSDETECT_H::getPlatform() != Linux) {
        QMessageBox::critical(nullptr, "Not supported", "Only linux is supported");
        return -1;
    }

    //Launcher::checkFoldersAndFiles(Steam::getETS2PrefixPath(Steam::getSteamInstallPath()), onProgressUpdate);
    NetworkingUtils::download(
        NetworkingUtils::DOWNLOAD,
        "/files/core_atsmp.dll",
        "/home/werwolf2303/Documents/GitHub/TruckersMPLauncherUnofficial/core_atsmp.dll",
        onError,
        onProgressUpdate
    );

    if(true) {
        return 0;
    }

    return app.exec();
}
