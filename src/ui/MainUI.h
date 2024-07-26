//
// Created by werwolf2303 on 6/30/24.
//

#ifndef MAINUI_H
#define MAINUI_H
#include "ui_MainUI.h"
#include "../install/Steam.h"

class MainUI : public QMainWindow {
    Q_OBJECT
public:
    MainUI(QWidget *parent, char *args[]);
private:
    Ui::MainWindow mainWindow;
    QStatusBar *statusBar;
    Steam::GameEntry selectedGame;
    std::vector<Steam::GameEntry> availableGames;
    QWebEngineView *webView;
    char *argv[];
    bool initEvents();
    bool initServerStatus();
    bool initVersions();
    bool initCommunity();
    bool initSocialMedia();
    bool initContactUs();
    bool initAccount();
    bool initSettings();
    bool initProgressbar();
    bool initWebView();
    bool initGameSelection();
    bool initPlay();
    int uiErrorHandler(ErrorHandling::ErrorType errorType, std::string message);
    void progressUpdate(int maximum, int minimum, int actual, std::string info, std::string downloadSpeed);
private slots:
    void gameSelected(int index);
    void initPlayClicked();
    void eventsClicked();
    void serverStatusClicked();
    void versionsClicked();
    void communityClicked();
    void socialMediaClicked();
    void contactUsClicked();
    void accountClicked();
    void settingsClicked();
};

#endif //MAINUI_H
