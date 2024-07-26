//
// Created by werwolf2303 on 6/30/24.
//

#include "MainUI.h"

#include <iostream>
#include <QApplication>
#include <QStatusBar>
#include <QMessageBox>
#include "../install/Steam.h"
#include "../launcher/Launcher.h"
#include <QGraphicsPixmapItem>
#include <thread>
#include <QtConcurrent>
#include "../events/Events.h"


void copyArray(char *dest[], char *src[], int size) {
    for (int i = 0; i < size; ++i) {
        dest[i] = new char[strlen(src[i]) + 1];
        strcpy(dest[i], src[i]);
    }
}

MainUI::MainUI(QWidget *parent, char *args[]) : QMainWindow(parent) {
    copyArray(argv, args, sizeof(args) / sizeof(args[0]));
    mainWindow.setupUi(this);

    this->setWindowTitle("TruckersMP Launcher for Linux - v0.1.0");
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    using std::get;

    Events::subscribe(Events::EventsList::onError, [this](std::any data) {
        std::tuple<ErrorHandling::ErrorType, std::string> unpacked = std::any_cast<std::tuple<ErrorHandling::ErrorType,
            std::string> >(data);
        uiErrorHandler(get<0>(unpacked), get<1>(unpacked));
    });

    Events::subscribe(Events::EventsList::onProgressUpdate, [this](std::any data) {
        std::tuple<int, int, int, std::string, std::string> unpacked = std::any_cast<std::tuple<int, int, int,
            std::string, std::string> >(data);
        progressUpdate(get<0>(unpacked), get<1>(unpacked), get<2>(unpacked), get<3>(unpacked), get<4>(unpacked));
    });

    statusBar = new QStatusBar;
    statusBar->setSizeGripEnabled(false);
    statusBar->setStyleSheet("QStatusBar { color: red; }");
    setStatusBar(statusBar);

    if (!initEvents()) QMessageBox::critical(nullptr, "Critical", "initEvents failed");
    if (!initServerStatus()) QMessageBox::critical(nullptr, "Critical", "initServerStatus failed");
    if (!initVersions()) QMessageBox::critical(nullptr, "Critical", "initVersions failed");
    if (!initCommunity()) QMessageBox::critical(nullptr, "Critical", "initCommunity failed");
    if (!initSocialMedia()) QMessageBox::critical(nullptr, "Critical", "initSocialMedia failed");
    if (!initContactUs()) QMessageBox::critical(nullptr, "Critical", "initContactUs failed");
    if (!initAccount()) QMessageBox::critical(nullptr, "Criticalsi", "initAccount failed");
    if (!initSettings()) QMessageBox::critical(nullptr, "Critical", "initSettings failed");
    if (!initProgressbar()) QMessageBox::critical(nullptr, "Critical", "initProgressbar failed");
    if (!initWebView()) QMessageBox::critical(nullptr, "Critical", "initWebView failed");
    if (!initGameSelection()) QMessageBox::critical(nullptr, "Critical", "initGameSelection failed");
    if (!initPlay()) QMessageBox::critical(nullptr, "Critical", "initPlay failed");
}


int MainUI::uiErrorHandler(ErrorHandling::ErrorType errorType, std::string message) {
    if (errorType == ErrorHandling::DESCRIPTIVE || errorType == ErrorHandling::DESCRIPTIVE) {
        statusBar->showMessage(QString("An error occurred: ").append(message.c_str()), 9999);
    } else {
        statusBar->showMessage(QString("An unknown error occurred"));
    }
    return 0;
}

bool MainUI::initEvents() {
    try {
        connect(mainWindow.eventsButton, &QPushButton::clicked, this, &MainUI::eventsClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initServerStatus() {
    try {
        connect(mainWindow.serverStatusButton, &QPushButton::clicked, this, &MainUI::serverStatusClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initVersions() {
    try {
        connect(mainWindow.versionsButton, &QPushButton::clicked, this, &MainUI::versionsClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initCommunity() {
    try {
        connect(mainWindow.communityButton, &QPushButton::clicked, this, &MainUI::communityClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initSocialMedia() {
    try {
        connect(mainWindow.socialMediaButton, &QPushButton::clicked, this, &MainUI::socialMediaClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initContactUs() {
    try {
        connect(mainWindow.contactUSButton, &QPushButton::clicked, this, &MainUI::contactUsClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initAccount() {
    try {
        connect(mainWindow.accountButton, &QPushButton::clicked, this, &MainUI::accountClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initSettings() {
    try {
        connect(mainWindow.settingsButton, &QPushButton::clicked, this, &MainUI::settingsClicked);
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initProgressbar() {
    try {
        mainWindow.progressDescription->setText("");
        return true;
    } catch (...) {
        return false;
    }
}

bool MainUI::initWebView() {
    try {
        webView = mainWindow.tabView;
        communityClicked();
        return webView != nullptr;
    } catch (...) {
        return false;
    }
}


bool MainUI::initGameSelection() {
    try {
        connect(mainWindow.gameSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &MainUI::gameSelected);
        std::vector<Steam::GameEntry> gameList = Steam::getGameList(Steam::getSteamInstallPath());
        for (auto &i: gameList) {
            availableGames.push_back(i);
            mainWindow.gameSelect->addItem(QString(i.name.c_str()));
        }
        selectedGame = availableGames[mainWindow.gameSelect->currentIndex()];
    } catch (...) {
        return false;
    }
    return true;
}

bool MainUI::initPlay() {
    try {
        connect(mainWindow.gameLaunch, &QPushButton::clicked, this, &MainUI::initPlayClicked);
    } catch (...) {
        return false;
    }
    return true;
}

void MainUI::gameSelected(int index) {
    selectedGame = availableGames[mainWindow.gameSelect->currentIndex()];
}

void MainUI::initPlayClicked() {
    QFuture<void> future = QtConcurrent::run([this] {
        if (!selectedGame.selectable) {
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, "Please start the game once to create the prefix"));
            return;
        }
        std::string steamPath = Steam::getSteamInstallPath();
        if (selectedGame.gameType == Steam::ETS2) {
            std::string prefixPath = Steam::getETS2PrefixPath(steamPath);
            std::string gamePath = Steam::getETS2InstallPath(steamPath);
            if (Launcher::checkFoldersAndFiles(prefixPath)) {
                if (Launcher::downloadTMPFiles(prefixPath, TruckersMPAPI::ETS2)) {
                    if (Launcher::verifyTMPFiles(prefixPath, TruckersMPAPI::ETS2)) {
                        Launcher::injectTruckersMP(Steam::getETS2PrefixPath(Steam::getSteamInstallPath()),
                                                   Steam::getETS2InstallPath(Steam::getSteamInstallPath()), argv);
                    }
                }
            }
        }
        if (selectedGame.gameType == Steam::ATS) {
            std::string prefixPath = Steam::getATSPrefixPath(steamPath);
            std::string gamePath = Steam::getATSInstallPath(steamPath);
            if (Launcher::checkFoldersAndFiles(prefixPath)) {
                if (Launcher::downloadTMPFiles(prefixPath, TruckersMPAPI::ATS)) {
                    if (Launcher::verifyTMPFiles(prefixPath, TruckersMPAPI::ATS)) {
                        Launcher::injectTruckersMP(Steam::getATSPrefixPath(Steam::getSteamInstallPath()),
                                                   Steam::getATSInstallPath(Steam::getSteamInstallPath()), argv);
                    }
                }
            }
        }
    });
}

void MainUI::accountClicked() {
}

void MainUI::communityClicked() {
    try {
        webView->load(QUrl("http://127.0.0.1:11232/community"));
    } catch (std::exception &e) {
        uiErrorHandler(ErrorHandling::DESCRIPTIVE, e.what());
    }
}

void MainUI::eventsClicked() {
    try {
        webView->load(QUrl("http://127.0.0.1:11232/events"));
    } catch (std::exception &e) {
        uiErrorHandler(ErrorHandling::DESCRIPTIVE, e.what());
    }
}

void MainUI::settingsClicked() {
    try {
        webView->load(QUrl("http://127.0.0.1:11232/settings"));
    } catch (std::exception &e) {
        uiErrorHandler(ErrorHandling::DESCRIPTIVE, e.what());
    }
}

void MainUI::versionsClicked() {
    try {
        webView->load(QUrl("http://127.0.0.1:11232/versions"));
    } catch (std::exception &e) {
        uiErrorHandler(ErrorHandling::DESCRIPTIVE, e.what());
    }
}

void MainUI::contactUsClicked() {
}

void MainUI::serverStatusClicked() {
    try {
        webView->load(QUrl("http://127.0.0.1:11232/serverstatus"));
    } catch (std::exception &e) {
        uiErrorHandler(ErrorHandling::DESCRIPTIVE, e.what());
    }
}

void MainUI::socialMediaClicked() {
}

void MainUI::progressUpdate(int maximum, int minimum, int actual, std::string info, std::string downloadSpeed) {
    QString text = QString(info.c_str()).append(" @ ").append(downloadSpeed).append("MBps");
    QMetaObject::invokeMethod(mainWindow.progressDescription, "setText", Q_ARG(QString, text));
    QMetaObject::invokeMethod(mainWindow.progressBar, "setMinimum", Q_ARG(int, minimum));
    QMetaObject::invokeMethod(mainWindow.progressBar, "setMaximum", Q_ARG(int, maximum));
    QMetaObject::invokeMethod(mainWindow.progressBar, "setValue", Q_ARG(int, actual));

}








