#include <iostream>
#include "api/TruckersMPAPI.h"
#include <QMessageBox>

#include "events/Events.h"
#include "react/FrontendServer.h"
#include "utils/OSDetect.h"
#include "ui/MainUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if(OSDETECT_H::getPlatform() != Linux) {
        QMessageBox::critical(nullptr, "Not supported", "Only linux is supported");
        return -1;
    }

    Events::registerEvent(Events::onError);
    Events::registerEvent(Events::onProgressUpdate);

    FrontendServer server;
    server.init();

    MainUI ui(nullptr, argv);
    try {
        ui.show();
    } catch (...) {
        QMessageBox::critical(nullptr, "Critical", "Failed to initialize main window");
        return -1;
    }

    if(!server.start()) {
        QMessageBox::critical(nullptr, "Critical", "Failed to initialize Server");
        return -1;
    }

    return app.exec() ;
}
