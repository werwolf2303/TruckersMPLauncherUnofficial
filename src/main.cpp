#include <iostream>
#include "api/TruckersMPAPI.h"
#include <QApplication>
#include <QMessageBox>
#include "install/Steam.h"
#include "utils/FileUtils.h"
#include "utils/OSDetect.h"

int onError(TruckersMPAPI::ErrorType errorType, std::string message) {
    std::cout << "Error happened in TruckersMP api (Descriptive? " << (errorType == TruckersMPAPI::DESCRIPTIVE || errorType == TruckersMPAPI::DESCRIPTIVE_API) << ") : " + message << "\n";

    return 0;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if(OSDETECT_H::getPlatform() != Linux) {
        QMessageBox::critical(nullptr, "Not supported", "Only linux is supported");
        return -1;
    }

    if(true) {
        return 0;
    }

    return app.exec();
}
